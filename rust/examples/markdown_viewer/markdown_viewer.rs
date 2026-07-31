mod markdown_viewer_ui;
#[path = "blocks/code_block_ui.rs"]
mod code_block_ui;
#[path = "blocks/heading_ui.rs"]
mod heading_ui;
#[path = "blocks/paragraph_ui.rs"]
mod paragraph_ui;
#[path = "blocks/table_block_ui.rs"]
mod table_block_ui;
#[path = "document_view/document_view_ui.rs"]
mod document_view_ui;

use code_block_ui::CodeBlockUI;
use document_view_ui::DocumentViewUI;
use heading_ui::HeadingUI;
use markdown_viewer_ui::*;
use paragraph_ui::ParagraphUI;
use std::collections::{BTreeMap, HashSet};
use std::fs;
use std::path::PathBuf;
use table_block_ui::TableBlockUI;

const PREFERRED_DOCUMENTS: [&str; 4] = [
    "overview.markdown",
    "styles.markdown",
    "tables.markdown",
    "code.markdown",
];

#[derive(Clone, Debug, Default, Eq, PartialEq)]
struct MarkdownBlock
{
    kind: String,
    level: usize,
    text: String,
    language: String,
}

fn trim(value: &str) -> String
{
    value.trim().to_string()
}

fn is_table_separator(line: &str) -> bool
{
    let value = line.trim();
    value.len() >= 3
        && value.starts_with('|')
        && value.ends_with('|')
        && value
            .chars()
            .all(|ch| matches!(ch, '|' | ' ' | '-' | ':'))
}

fn flush_paragraph(paragraph: &mut Vec<String>, blocks: &mut Vec<MarkdownBlock>)
{
    let value = trim(&paragraph.join("\n"));
    if !value.is_empty()
    {
        blocks.push(MarkdownBlock
        {
            kind: "paragraph".to_string(),
            text: value,
            ..Default::default()
        });
    }
    paragraph.clear();
}

fn parse_markdown(text: &str) -> Vec<MarkdownBlock>
{
    let lines: Vec<&str> = text.split('\n').collect();
    let mut blocks = Vec::new();
    let mut paragraph = Vec::new();
    let mut index = 0;
    while index < lines.len()
    {
        let line = lines[index];
        if line.trim().is_empty()
        {
            flush_paragraph(&mut paragraph, &mut blocks);
            index += 1;
        }
        else if let Some(language) = line.strip_prefix("```")
        {
            flush_paragraph(&mut paragraph, &mut blocks);
            let language = if language.trim().is_empty()
            {
                "text".to_string()
            }
            else
            {
                trim(language)
            };
            index += 1;
            let mut code = Vec::new();
            while index < lines.len() && !lines[index].starts_with("```")
            {
                code.push(lines[index]);
                index += 1;
            }
            if index < lines.len()
            {
                index += 1;
            }
            blocks.push(MarkdownBlock
            {
                kind: "code".to_string(),
                text: code.join("\n"),
                language,
                ..Default::default()
            });
        }
        else if line.starts_with('#')
        {
            flush_paragraph(&mut paragraph, &mut blocks);
            let level = line.chars().take_while(|ch| *ch == '#').count();
            let text = trim(&line[level..]);
            blocks.push(MarkdownBlock
            {
                kind: "heading".to_string(),
                level,
                text,
                ..Default::default()
            });
            index += 1;
        }
        else if line.starts_with('|')
            && index + 1 < lines.len()
            && is_table_separator(lines[index + 1])
        {
            flush_paragraph(&mut paragraph, &mut blocks);
            let mut table = vec![line, lines[index + 1]];
            index += 2;
            while index < lines.len() && lines[index].starts_with('|')
            {
                table.push(lines[index]);
                index += 1;
            }
            blocks.push(MarkdownBlock
            {
                kind: "table".to_string(),
                text: table.join("\n"),
                ..Default::default()
            });
        }
        else
        {
            paragraph.push(line.to_string());
            index += 1;
        }
    }
    flush_paragraph(&mut paragraph, &mut blocks);
    blocks
}

fn docs_dir() -> PathBuf
{
    PathBuf::from(env!("CARGO_MANIFEST_DIR")).join("docs")
}

fn load_documents() -> BTreeMap<String, String>
{
    let directory = docs_dir();
    let mut documents = BTreeMap::new();
    for name in PREFERRED_DOCUMENTS
    {
        let path = directory.join(name);
        if path.exists()
        {
            documents.insert(
                name.to_string(),
                fs::read_to_string(path).unwrap_or_default(),
            );
        }
    }
    if let Ok(entries) = fs::read_dir(directory)
    {
        for entry in entries.flatten()
        {
            let path = entry.path();
            if path.extension().and_then(|value| value.to_str()) != Some("markdown")
            {
                continue;
            }
            let Some(name) = path.file_name().and_then(|value| value.to_str()) else
            {
                continue;
            };
            documents
                .entry(name.to_string())
                .or_insert_with(|| fs::read_to_string(path).unwrap_or_default());
        }
    }
    documents
}

fn highlighted_python_spans(source: &str) -> Vec<uimd::LabelSpan>
{
    let keywords: HashSet<&str> = [
        "False", "None", "True", "and", "as", "assert", "async", "await",
        "break", "class", "continue", "def", "del", "elif", "else", "except",
        "finally", "for", "from", "global", "if", "import", "in", "is",
        "lambda", "nonlocal", "not", "or", "pass", "raise", "return", "try",
        "while", "with", "yield",
    ]
    .into_iter()
    .collect();
    let mut spans = Vec::new();
    let mut cursor = 0;
    let mut index = 0;
    while index < source.len()
    {
        let Some(ch) = source[index..].chars().next() else { break };
        if !ch.is_ascii_alphanumeric() && ch != '_'
        {
            index += ch.len_utf8();
            continue;
        }
        let start = index;
        while index < source.len()
        {
            let Some(next) = source[index..].chars().next() else { break };
            if !next.is_ascii_alphanumeric() && next != '_'
            {
                break;
            }
            index += next.len_utf8();
        }
        let word = &source[start..index];
        if !keywords.contains(word)
        {
            continue;
        }
        if start > cursor
        {
            spans.push(uimd::LabelSpan::new(&source[cursor..start], "", ""));
        }
        spans.push(uimd::LabelSpan::new(word, "#c084fc", ""));
        cursor = index;
    }
    if cursor < source.len()
    {
        spans.push(uimd::LabelSpan::new(&source[cursor..], "", ""));
    }
    spans
}

fn block_window(block: &MarkdownBlock) -> uimd::GeneratedWindow
{
    match block.kind.as_str()
    {
        "heading" =>
        {
            let ui = HeadingUI::new();
            let level = block.level.clamp(1, 3);
            let color = match level
            {
                1 => "#facc15",
                2 => "#93c5fd",
                _ => "#a7f3d0",
            };
            ui.heading_text.borrow_mut().set_spans(vec![
                uimd::LabelSpan::new("#".repeat(level), "#64748b", ""),
                uimd::LabelSpan::new(" ", "", ""),
                uimd::LabelSpan::new(&block.text, color, ""),
            ]);
            ui.base
        }
        "code" =>
        {
            let ui = CodeBlockUI::new();
            ui.lang.borrow_mut().set_text(if block.language.is_empty()
            {
                "text"
            }
            else
            {
                &block.language
            });
            ui.code
                .borrow_mut()
                .set_spans(highlighted_python_spans(&block.text));
            ui.base
        }
        "table" =>
        {
            let ui = TableBlockUI::new();
            ui.table.borrow_mut().set_text(&block.text);
            ui.base
        }
        _ =>
        {
            let ui = ParagraphUI::new();
            ui.text.borrow_mut().set_text(&block.text);
            ui.base
        }
    }
}

struct MarkdownViewerApp
{
    documents: BTreeMap<String, String>,
}

impl MarkdownViewerApp
{
    fn new() -> Self
    {
        Self { documents: load_documents() }
    }

    fn initialize(&mut self, ui: &mut MarkdownViewerUI)
    {
        let mut names = Vec::new();
        for preferred in PREFERRED_DOCUMENTS
        {
            if self.documents.contains_key(preferred)
            {
                names.push(preferred.to_string());
            }
        }
        for name in self.documents.keys()
        {
            if !names.contains(name)
            {
                names.push(name.clone());
            }
        }
        ui.docs.borrow_mut().set_options(names);
        ui.docs.borrow_mut().set_selected_index(0);
        let document = DocumentViewUI::new();
        ui.viewer.borrow_mut().set_child_window(document.base);
        ui.base.set_runtime_options(uimd::GeneratedWindowRuntimeOptions
        {
            initial_focus_name: "docs".to_string(),
            start_in_edit_mode: true,
            ..Default::default()
        });
        self.show_selected(ui);
    }

    fn show_selected(&self, ui: &mut MarkdownViewerUI)
    {
        let name = ui.docs.borrow().selected_value();
        if name.is_empty()
        {
            return;
        }
        let blocks = parse_markdown(
            self.documents
                .get(&name)
                .map(String::as_str)
                .unwrap_or_default(),
        );
        {
            let mut viewer = ui.viewer.borrow_mut();
            let document = viewer
                .child_window_mut()
                .expect("generated document view child must be initialized");
            document.clear_children();
            document.set_auto_scroll(false);
            for (index, block) in blocks.iter().enumerate()
            {
                document.add_named_child_window(
                    format!("viewer.blocks[{index}]"),
                    block_window(block),
                );
            }
        }
        ui.status.borrow_mut().set_text(format!(
            "{name} - {} blocks - arrows preview",
            blocks.len(),
        ));
    }

    fn scroll(&self, ui: &MarkdownViewerUI, key: &str)
    {
        let mut viewer = ui.viewer.borrow_mut();
        let Some(document) = viewer.child_window_mut() else { return };
        document.handle_key(key);
    }
}

impl MarkdownViewerUIEvents for MarkdownViewerApp
{
    fn on_focus_changed(&mut self, ui: &mut MarkdownViewerUI, name: &str, focused: bool)
    {
        if !focused
        {
            return;
        }
        if name == "viewer"
        {
            ui.status.borrow_mut().set_text(
                "viewer focus - Enter scroll mode, Esc navigation, arrows/PageUp/PageDown/Home/End scroll",
            );
        }
        else if matches!(name, "up_btn" | "down_btn")
        {
            ui.status
                .borrow_mut()
                .set_text("buttons scroll the viewer; Right focuses viewer");
        }
    }

    fn on_up_btn_click(&mut self, ui: &mut MarkdownViewerUI)
    {
        self.scroll(ui, "ArrowUp");
    }

    fn on_down_btn_click(&mut self, ui: &mut MarkdownViewerUI)
    {
        self.scroll(ui, "ArrowDown");
    }

    fn on_close_btn_click(&mut self, ui: &mut MarkdownViewerUI)
    {
        ui.base.request_close();
    }

    fn on_docs_selectionchange(&mut self, ui: &mut MarkdownViewerUI, _value: &[String])
    {
        self.show_selected(ui);
    }
}

fn main()
{
    let code = uimd::run_generated_app_main(||
    {
        let mut ui = MarkdownViewerUI::new();
        let mut app = MarkdownViewerApp::new();
        app.initialize(&mut ui);
        ui.run(&mut app, &std::env::args().collect::<Vec<_>>())
    });
    std::process::exit(code);
}

#[cfg(test)]
mod tests
{
    use super::*;

    #[test]
    fn parser_matches_reference_block_counts()
    {
        let documents = load_documents();
        let expected = [
            ("overview.markdown", 47),
            ("styles.markdown", 46),
            ("tables.markdown", 32),
            ("code.markdown", 29),
        ];
        for (name, count) in expected
        {
            assert_eq!(
                parse_markdown(documents.get(name).expect("reference markdown document")).len(),
                count,
                "{name}",
            );
        }
    }

    #[test]
    fn parser_recognizes_each_reference_block_kind()
    {
        let blocks = parse_markdown(
            "# H\n\ntext\n\n```python\nprint(1)\n```\n\n| A |\n| - |\n| B |",
        );
        assert_eq!(blocks.len(), 4);
        assert_eq!(blocks[0].kind, "heading");
        assert_eq!(blocks[1].kind, "paragraph");
        assert_eq!(blocks[2].kind, "code");
        assert_eq!(blocks[3].kind, "table");
    }
}
