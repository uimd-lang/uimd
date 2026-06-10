# Custom UI Implementation Plan

## Ciel

Dokoncit podporu custom UI elementov tak, aby element v `.md` subore mohol pouzit vlastny typ odvodeny z ineho `.md` UI suboru.

Konkretne testovacie pouzitie:

- `examples/chat/chat.md` nebude mat `chat` ako `type: uielement`
- `chat` bude mat `type: chat_panel`
- `chat_panel` bude definovany cez `examples/chat/chat_panel.md`
- pri potvrdeni spravy z `prompt` sa sprava dynamicky vlozi do chat panelu

## Stav

| Krok | Stav | Poznamka |
| --- | --- | --- |
| 1. Zmapovat aktualny stav `uielement` | DONE | `uielement` existuje, ale je to hlavne textovy placeholder; `layout_class` sa negeneruje ani nenapaja. |
| 2. Pouzit `chat_panel.md` ako cielovy custom UI priklad | DONE | `examples/chat/chat_panel.md` existuje ako novy nepridany SVN subor. |
| 3. Zmenit `examples/chat/chat.md` na `type: chat_panel` | DONE | `chat` pouziva konkretny custom typ `chat_panel`, nie vseobecny `uielement`. |
| 4. Rozsirit compiler o custom UI typy | DONE | Generator rozpoznava non-builtin typy a mapuje ich na runtime `uielement` s `layout_class`. |
| 5. Rozsirit runtime custom elementu | DONE | Custom element drzi zivu child UI instanciu namiesto docasneho objektu pri kazdom renderi. |
| 6. Prepojit lifecycle a rozmery | DONE | `open`, `resize`, `_app`, parent, layout geometry a render prechadzaju do child UI. |
| 7. Vlozit spravy dynamicky do `chat_panel` | DONE | `Chat._on_send()` vola `self.chat.append_message(...)`; panel uklada text do vnutorneho `messages` labelu. |
| 8. Doplnit testy | DONE | Pridany compiler test pre custom typ, runtime test pre zivu child instanciu a label newline regresia. |
| 9. Skompilovat a overit chat priklad | DONE | Skompilovane `chat_panel.md` aj `chat.md`; overeny neinteraktivny send do child panelu. |

## Navrhovany dizajn

### Builtin vs custom typy

Builtin typy ostanu:

- `label`
- `textinput`
- `numberinput`
- `button`
- `checkbox`
- `combobox`
- `listbox`
- `uielement`

Ak `members.<name>.type` nie je builtin typ, compiler ho bude brat ako custom UI typ.

Priklad v sekcii `## Members`:

```yaml
chat:
  type: chat_panel
```

Compiler bude hladat custom UI modul v tom istom adresari ako aktualny `.md` subor:

- `examples/chat/chat_panel.md`
- po kompilacii `examples/chat/chat_panel_ui.py`
- aplikacna trieda `ChatPanel` v `examples/chat/chat_panel.py`

### Generovany kod

Pre custom typ `chat_panel` generator vygeneruje import:

```python
from chat_panel import ChatPanel
```

A element vytvori napr. takto:

```python
self.chat = self.create_element("chat", "uielement", layout_class=ChatPanel)
```

Pouzije sa interny runtime typ `uielement`, ale zdrojovy `.md` bude pisat konkretne `type: chat_panel`. Importuje sa aplikacna trieda, aby custom UI mohlo mat vlastne metody ako `append_message()`.

### Runtime spravanie

`UIElementReusable` bude mat jednu zivu child instanciu:

- vytvori ju pri inicializacii alebo pri `open`
- nastavi jej `_app`
- nastavi jej parent
- pri `resize` jej posunie aktualnu sirku/vysku elementu
- pri `render` vrati render child instancie orezany/paddingnuty na rozmery elementu

Docasne vytvaranie child instancie v `render()` treba odstranit.

### Dynamicke spravy v chat paneli

`chat_panel.md` bude sluzit ako panel, ktory vie zobrazovat dynamicky obsah. Prvy krok moze byt jednoduchy:

- child UI bude mat label alebo panelovu oblast pre spravy
- `Chat._on_send()` zavola metodu na `self.chat`, napr. `append_message(line)`
- `UIElementReusable` tuto metodu deleguje na child instanciu, alebo poskytne `child` getter

Konzervativna prva verzia:

```python
self.chat.child.append_message(line)
```

alebo:

```python
self.chat.append_message(line)
```

Vyber treba spravit podla toho, ktory tvar bude jednoduchsie testovat a udrzi menej duplicity.

## Test plan

1. Skompilovat custom panel:

```bash
./uimd generate examples/chat/chat_panel.md --target python
```

2. Skompilovat chat:

```bash
./uimd generate examples/chat/chat.md --target python
```

3. Spustit testy:

```bash
python3 -m pytest
```

4. Manualne overit chat:

```bash
python3 examples/chat/chat.py
```

Overenie:

- `chat.md` pouziva `type: chat_panel`
- chat sa spusti bez import/runtime chyby
- prompt ostava v edit mode
- po napisani spravy a potvrdeni Enterom sa sprava zobrazi v chat paneli
- `/settings` spravanie ostava funkcne

## Vysledok implementacie

- `examples/chat/chat.md` pouziva `type: chat_panel`.
- `compiler/code_generator.py` mapuje non-builtin typy na runtime `uielement` s importovanou aplikacnou triedou.
- `runtime/elements.py` drzi custom child UI ako zivu instanciu.
- `examples/chat/chat_panel.py` poskytuje `clear_messages()` a `append_message()`.
- `examples/chat/chat.py` vklada spravy do `chat_panel` cez `self.chat.append_message(...)`.
- `Label` respektuje explicitne newline riadky pri viacriadkovom renderi.
- Testy: `229 passed`.

## Otvorene rozhodnutia

- Ci custom typ ma vzdy hladat `<type>.md` v rovnakom adresari, alebo podporit aj explicitny `source`.
- Verejne API aktualne podporuje delegovane metody priamo na `self.chat` aj pristup cez `self.chat.child`.
- Dynamicky zoznam sprav je v prvej verzii reprezentovany cez existujuci viacriadkovy `Label`.
