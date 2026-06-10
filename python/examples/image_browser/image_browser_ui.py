"""Auto-generated UI code for image_browser_ui.py - DO NOT EDIT MANUALLY."""

from uimd.runtime import UIWindow

from image_button import ImageButton
from gallery_mosaic import GalleryMosaic
from image_list_button import ImageListButton


class ImageBrowserUI(UIWindow):
    """Generated UI window from markdown definition."""

    _mcp_enabled = True
    _mcp_source_md = '# Image Browser\n\n## Metadata\n\n```yaml\nformat: uimd\nformat-version: 1\nkind: window\ndescription: "Image browser with a thumbnail sidebar and full-size image panel."\ntags: [example, image, viewhost]\n```\n\n## Definition\n\n```yaml\nuses:\n  - image_view\n  - gallery_view\n  - gallery_mosaic\n  - image_button\n  - image_list_button\n  - image_list_view\n  - image_show_dialog\n```\n\n## Members\n\n```yaml\ntitle:\n  type: label\n  text: "Image Browser"\n  description: "Window title."\n  expose: false\n\nclose_btn:\n  type: button\n  title: Quit\n  description: "Close the image browser."\n\nrender_label:\n  type: label\n  text: "Render"\n  description: "Label above the render mode selector."\n  expose: false\n\nmode_select:\n  type: combobox\n  options: [Normal, Fallback]\n  selected_item: Normal\n  description: "Switch between Sixel and fallback image rendering."\n\nsidebar_title:\n  type: label\n  text: "Photos"\n  description: "Sidebar section label."\n  expose: false\n\ncamera_thumb:\n  type: image_button\n  description: "Camera thumbnail. Click to view full size."\n\nastro_thumb:\n  type: image_button\n  description: "Astronaut thumbnail. Click to view full size."\n\ncoffee_thumb:\n  type: image_button\n  description: "Coffee thumbnail. Click to view full size."\n\ngallery_label:\n  type: label\n  text: "Gallery"\n  description: "Label above the gallery mosaic."\n  expose: false\n\nimage_list_label:\n  type: label\n  text: "Image items"\n  description: "List preview label."\n  expose: false\n\ngallery_mosaic:\n  type: gallery_mosaic\n  description: "Click any image to open the full scrollable gallery."\n\nimage_list_btn:\n  type: image_list_button\n  description: "Click to open the image item list."\n\nspacer:\n  type: label\n  text: ""\n  description: "Spacer that absorbs leftover sidebar height."\n  expose: false\n\nmain:\n  type: viewhost\n  description: "Full-size image panel."\n```\n\n## Style\n\n```yaml\ninclude: dark\nthis:\n  border-width: 0\ncell:\n  padding: 1\n@header:\n  background: "#1f2937"\n  padding: 0\n@sidebar:\n  background: "#172033"\n  padding: 0\n@content:\n  background: "#030712"\n@title:\n  color: "#ffffff"\n  text-align: center\n@close_btn:\n  background: "#7f1d1d"\n  focus-background: "#dc2626"\n  color: "#ffffff"\n@render_label:\n  color: "#93c5fd"\n@sidebar_title:\n  color: "#93c5fd"\n@gallery_label:\n  color: "#93c5fd"\n@image_list_label:\n  color: "#93c5fd"\ncombobox:\n  background: "#0b1220"\n  color: "#e5e7eb"\n  focus-background: "#25364f"\n  edit-background: "#0b1220"\n  selected-background: "#2563eb"\n  selected-color: "#ffffff"\nviewhost:\n  background: "#030712"\n```\n\n## User Interface\n\n```ui\n+--header-**-------------------------------------------------------------------+\n| title..**....................................................... close_btn.. |\n+--sidebar-------------------+--content-**-------------------------------------+\n| render_label.............. | main..**....................................... |\n* mode_select............... | ............................................... |\n*                            | *.............................................. |\n| sidebar_title............. | *.............................................. |\n| camera_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| astro_thumb............... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| coffee_thumb.............. | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| gallery_label............. | ............................................... |\n| gallery_mosaic............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| image_list_label.......... | ............................................... |\n| image_list_btn............ | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n| .......................... | ............................................... |\n* spacer.................... | ............................................... |\n* .......................... | ............................................... |\n| .......................... | ............................................... |\n+----------------------------+-------------------------------------------------+\n```\n'
    _mcp_agent_metadata = {'window': {'description': 'Image browser with a thumbnail sidebar and full-size image panel.'}, 'elements': {'title': {'role': 'text', 'description': 'Window title.', 'expose': False}, 'close_btn': {'role': 'action', 'description': 'Close the image browser.', 'expose': True}, 'render_label': {'role': 'text', 'description': 'Label above the render mode selector.', 'expose': False}, 'mode_select': {'role': 'selection', 'description': 'Switch between Sixel and fallback image rendering.', 'expose': True}, 'sidebar_title': {'role': 'text', 'description': 'Sidebar section label.', 'expose': False}, 'camera_thumb': {'role': 'text', 'description': 'Camera thumbnail. Click to view full size.', 'expose': True}, 'astro_thumb': {'role': 'text', 'description': 'Astronaut thumbnail. Click to view full size.', 'expose': True}, 'coffee_thumb': {'role': 'text', 'description': 'Coffee thumbnail. Click to view full size.', 'expose': True}, 'gallery_label': {'role': 'text', 'description': 'Label above the gallery mosaic.', 'expose': False}, 'image_list_label': {'role': 'text', 'description': 'List preview label.', 'expose': False}, 'gallery_mosaic': {'role': 'text', 'description': 'Click any image to open the full scrollable gallery.', 'expose': True}, 'image_list_btn': {'role': 'text', 'description': 'Click to open the image item list.', 'expose': True}, 'spacer': {'role': 'text', 'description': 'Spacer that absorbs leftover sidebar height.', 'expose': False}, 'main': {'role': 'text', 'description': 'Full-size image panel.', 'expose': True}}}
    _mcp_app_tools = {}

    def __init__(self):
        super().__init__(
            title="Image Browser",
            layout=COMPILED_LAYOUT,
            members=COMPILED_MEMBERS,
            style=COMPILED_STYLE
        )
        self.focusable = False
        self._metadata_kind = 'window'
        if self._metadata_kind == "dialog":
            self.mode = "dialog"

        self.astro_thumb = self.create_element("astro_thumb", "uielement", layout_class=ImageButton)
        self.camera_thumb = self.create_element("camera_thumb", "uielement", layout_class=ImageButton)
        self.close_btn = self.create_element("close_btn", "button", title='Quit')
        self.coffee_thumb = self.create_element("coffee_thumb", "uielement", layout_class=ImageButton)
        self.gallery_label = self.create_element("gallery_label", "label", text='Gallery')
        self.gallery_mosaic = self.create_element("gallery_mosaic", "uielement", layout_class=GalleryMosaic)
        self.image_list_btn = self.create_element("image_list_btn", "uielement", layout_class=ImageListButton)
        self.image_list_label = self.create_element("image_list_label", "label", text='Image items')
        self.main = self.create_element("main", "viewhost", )
        self.mode_select = self.create_element("mode_select", "combobox", options=['Normal', 'Fallback'], selected_item='Normal')
        self.render_label = self.create_element("render_label", "label", text='Render')
        self.sidebar_title = self.create_element("sidebar_title", "label", text='Photos')
        self.spacer = self.create_element("spacer", "label", text='')
        self.title_element = self.create_element("title", "label", text='Image Browser')
    def on_close_btn_click(self):
        pass

    def on_mode_select_change(self, value):
        pass


    def _dispatch_element_changed(self, element, value):
        if element is self.close_btn:
            self.on_close_btn_click()
        super()._dispatch_element_changed(element, value)

    def _dispatch_confirmed(self, element, value):
        pass
        super()._dispatch_confirmed(element, value)

    def _dispatch_selection_changed(self, element, value):
        if element is self.mode_select:
            self.on_mode_select_change(value)
        super()._dispatch_selection_changed(element, value)


COMPILED_MEMBERS = {
    "title": {
        "type": 'label',
        "text": 'Image Browser',
        "description": 'Window title.',
        "expose": False,
    },
    "close_btn": {
        "type": 'button',
        "title": 'Quit',
        "description": 'Close the image browser.',
    },
    "render_label": {
        "type": 'label',
        "text": 'Render',
        "description": 'Label above the render mode selector.',
        "expose": False,
    },
    "mode_select": {
        "type": 'combobox',
        "options": ['Normal', 'Fallback'],
        "selected_item": 'Normal',
        "description": 'Switch between Sixel and fallback image rendering.',
    },
    "sidebar_title": {
        "type": 'label',
        "text": 'Photos',
        "description": 'Sidebar section label.',
        "expose": False,
    },
    "camera_thumb": {
        "type": 'image_button',
        "description": 'Camera thumbnail. Click to view full size.',
    },
    "astro_thumb": {
        "type": 'image_button',
        "description": 'Astronaut thumbnail. Click to view full size.',
    },
    "coffee_thumb": {
        "type": 'image_button',
        "description": 'Coffee thumbnail. Click to view full size.',
    },
    "gallery_label": {
        "type": 'label',
        "text": 'Gallery',
        "description": 'Label above the gallery mosaic.',
        "expose": False,
    },
    "image_list_label": {
        "type": 'label',
        "text": 'Image items',
        "description": 'List preview label.',
        "expose": False,
    },
    "gallery_mosaic": {
        "type": 'gallery_mosaic',
        "description": 'Click any image to open the full scrollable gallery.',
    },
    "image_list_btn": {
        "type": 'image_list_button',
        "description": 'Click to open the image item list.',
    },
    "spacer": {
        "type": 'label',
        "text": '',
        "description": 'Spacer that absorbs leftover sidebar height.',
        "expose": False,
    },
    "main": {
        "type": 'viewhost',
        "description": 'Full-size image panel.',
    },
}

COMPILED_STYLE = {
    "this": {
        "background": '#162033',
        "border-color": 'transparent',
        "border-width": 0,
    },
    "label": {
        "background": 'transparent',
        "color": '#cbd5e1',
        "cursor-background": '#b8d7ff',
        "cursor-color": '#000000',
    },
    "@dialog_header": {
        "text-align": 'center',
        "background": '#1d314c',
    },
    "@window_header": {
        "text-align": 'center',
        "background": '#1d314c',
    },
    "infolabel": {
        "background": 'transparent',
        "color": '#b8c4d8',
    },
    "textinput": {
        "background": '#0d1524',
        "color": '#e5e7eb',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#243a5c',
        "focus-color": '#ffffff',
        "edit-background": '#2d456d',
        "edit-color": '#ffffff',
        "cursor-background": '#ffffff',
        "cursor-color": '#0e1117',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "numberinput": {
        "background": '#0d1524',
        "color": '#e5e7eb',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#243a5c',
        "focus-color": '#ffffff',
        "edit-background": '#2d456d',
        "edit-color": '#ffffff',
        "cursor-background": '#ffffff',
        "cursor-color": '#0e1117',
    },
    "combobox": {
        "background": '#0b1220',
        "color": '#e5e7eb',
        "disabled-background": '#0b1220',
        "disabled-color": '#64748b',
        "focus-background": '#25364f',
        "focus-color": '#ffffff',
        "edit-background": '#0b1220',
        "edit-color": '#e5e7eb',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "listbox": {
        "background": '#0d1524',
        "color": '#cbd5e1',
        "disabled-color": '#64748b',
        "focus-background": '#1d2f4d',
        "focus-color": '#ffffff',
        "edit-background": '#243a5c',
        "edit-color": '#cbd5e1',
        "selected-background": '#2563eb',
        "selected-color": '#ffffff',
    },
    "uielement": {
        "focus-background": '#ffffff14',
        "edit-background": '#ffffff18',
        "selected-background": '#ffffff26',
        "selected-color": '#ffffff',
    },
    "uiscrollview": {
        "focus-background": '#ffffff14',
        "edit-background": '#ffffff18',
        "selected-background": '#ffffff26',
        "selected-color": '#ffffff',
    },
    "checkbox": {
        "background": 'transparent',
        "color": '#b8c4d8',
        "disabled-color": '#64748b',
        "focus-background": '#ffffff44',
        "focus-color": '#ffffff',
        "checked-background": 'transparent',
        "checked-color": '#44cc88',
        "unchecked-background": 'transparent',
    },
    "button": {
        "background": '#334155',
        "color": '#e2e8f0',
        "disabled-background": '#1f2937',
        "disabled-color": '#64748b',
        "focus-background": '#2b5f9f',
        "focus-color": '#ffffff',
    },
    "cell": {
        "padding": 1,
    },
    "@header": {
        "background": '#1f2937',
        "padding": 0,
    },
    "@sidebar": {
        "background": '#172033',
        "padding": 0,
    },
    "@content": {
        "background": '#030712',
    },
    "@title": {
        "color": '#ffffff',
        "text-align": 'center',
    },
    "@close_btn": {
        "background": '#7f1d1d',
        "focus-background": '#dc2626',
        "color": '#ffffff',
    },
    "@render_label": {
        "color": '#93c5fd',
    },
    "@sidebar_title": {
        "color": '#93c5fd',
    },
    "@gallery_label": {
        "color": '#93c5fd',
    },
    "@image_list_label": {
        "color": '#93c5fd',
    },
    "viewhost": {
        "background": '#030712',
    },
}

COMPILED_LAYOUT = [
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 29,
        'cell_chars_width': 49,
        'cell_chars_height': 41,
        'cell_name': 'content',
        'cell_height': 41,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'height_mode': 'expanded',
        'chars_width': 47,
        'chars_height': 41,
        'margin_right': 1,
        'type': 'viewhost',
        'name': 'main'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 40,
        'height': 1,
        'type': 'label',
        'name': 'render_label'
    },
    {
        'row': 0,
        'col': 1,
        'relative_col': 1,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'width_mode': 'expanded',
        'chars_width': 64,
        'chars_height': 1,
        'margin_right': 13,
        'height': 1,
        'type': 'label',
        'name': 'title'
    },
    {
        'row': 0,
        'col': 66,
        'relative_col': 66,
        'cell_row': 0,
        'cell_col': 0,
        'cell_chars_width': 78,
        'cell_chars_height': 1,
        'cell_name': 'header',
        'cell_height': 1,
        'cell_width_mode': 'expanded',
        'cell_height_mode': 'auto',
        'chars_width': 11,
        'chars_height': 1,
        'margin_right': 1,
        'width': 11,
        'height': 1,
        'type': 'button',
        'name': 'close_btn'
    },
    {
        'row': 1,
        'col': 1,
        'relative_row': 1,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 39,
        'height': 1,
        'type': 'combobox',
        'name': 'mode_select'
    },
    {
        'row': 3,
        'col': 1,
        'relative_row': 3,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 37,
        'height': 1,
        'type': 'label',
        'name': 'sidebar_title'
    },
    {
        'row': 4,
        'col': 1,
        'relative_row': 4,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 6,
        'margin_right': 1,
        'margin_bottom': 31,
        'height': 6,
        'type': 'image_button',
        'name': 'camera_thumb'
    },
    {
        'row': 10,
        'col': 1,
        'relative_row': 10,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 6,
        'margin_right': 1,
        'margin_bottom': 25,
        'height': 6,
        'type': 'image_button',
        'name': 'astro_thumb'
    },
    {
        'row': 16,
        'col': 1,
        'relative_row': 16,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 6,
        'margin_right': 1,
        'margin_bottom': 19,
        'height': 6,
        'type': 'image_button',
        'name': 'coffee_thumb'
    },
    {
        'row': 22,
        'col': 1,
        'relative_row': 22,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 18,
        'height': 1,
        'type': 'label',
        'name': 'gallery_label'
    },
    {
        'row': 23,
        'col': 1,
        'relative_row': 23,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 9,
        'margin_right': 1,
        'margin_bottom': 9,
        'height': 9,
        'type': 'gallery_mosaic',
        'name': 'gallery_mosaic'
    },
    {
        'row': 32,
        'col': 1,
        'relative_row': 32,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 1,
        'margin_right': 1,
        'margin_bottom': 8,
        'height': 1,
        'type': 'label',
        'name': 'image_list_label'
    },
    {
        'row': 33,
        'col': 1,
        'relative_row': 33,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 5,
        'margin_right': 1,
        'margin_bottom': 3,
        'height': 5,
        'type': 'image_list_button',
        'name': 'image_list_btn'
    },
    {
        'row': 38,
        'col': 1,
        'relative_row': 38,
        'relative_col': 1,
        'cell_row': 2,
        'cell_col': 0,
        'cell_chars_width': 28,
        'cell_chars_height': 41,
        'cell_name': 'sidebar',
        'cell_width': 28,
        'cell_width_mode': 'auto',
        'cell_height_mode': 'expanded',
        'width_mode': 'expanded',
        'chars_width': 26,
        'chars_height': 3,
        'margin_right': 1,
        'height': 3,
        'type': 'label',
        'name': 'spacer'
    }
]
