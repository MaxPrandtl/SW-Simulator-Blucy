from controller.Controller import Controller


theme = {
    "dark": {
        "ACTIVE_COLOR": "#03273D",
        "COLOR": "#193849",
        "HOVER_COLOR": "#192E39",
    },
    "light": {
        "ACTIVE_COLOR": "#6DAACD",
        "COLOR": "#D1D9DE",
        "HOVER_COLOR": "#96B3C5",
    },
}


class MenuHandler:
    def __init__(self, view, controller: Controller):
        self.view = view
        self.controller = controller
        self.current_index = 0
        self.selectors = [
            self.view.menu_status_selector,
            self.view.menu_logger_selector,
            self.view.menu_sub_selector,
            self.view.menu_data_selector,
            self.view.menu_light_selector,
            self.view.menu_settings_selector,
        ]

        self.__bind_menu_events()

    def __handle_button(self, index):
        self.refresh_colors(index)

        # Update settings
        if self.current_index == 5:
            self.controller.settings_handler.fill_settings(self.controller.get_all_config())

    def refresh_colors(self, index=None):
        for selector in self.selectors:
            selector.setStyleSheet(
                "QPushButton {\n"
                f"	background-color: {theme[self.controller.get_config('color-theme')]['COLOR']};\n"
                "}\n"
                "QPushButton:hover {\n"
                "	\n"
                f"	background-color: {theme[self.controller.get_config('color-theme')]['HOVER_COLOR']};\n"
                "}"
            )

        if index is not None:
            self.current_index = index

        self.selectors[self.current_index].setStyleSheet(
            f"background-color: {theme[self.controller.get_config('color-theme')]['ACTIVE_COLOR']};"
        )
        self.view.main_content_widget.setCurrentIndex(self.current_index)

    def __bind_menu_events(self):
        self.selectors[0].clicked.connect(lambda: self.__handle_button(0))
        self.selectors[1].clicked.connect(lambda: self.__handle_button(1))
        self.selectors[2].clicked.connect(lambda: self.__handle_button(2))
        self.selectors[3].clicked.connect(lambda: self.__handle_button(3))
        self.selectors[4].clicked.connect(lambda: self.__handle_button(4))
        self.selectors[5].clicked.connect(lambda: self.__handle_button(5))
