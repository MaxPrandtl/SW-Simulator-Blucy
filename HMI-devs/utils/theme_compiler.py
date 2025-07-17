class ThemeCompiler:
    @staticmethod
    def compile(theme: str, qss_file: str = "style.txt"):
        style = ""

        # Read all file
        with open(f"./theme/{qss_file}", "r") as f:
            style = f.read()

        variables = {}
        with open(f"./theme/{theme}_theme.txt", "r") as f:
            for line in f:
                if line.startswith("$"):
                    tokens = [token.strip() for token in line.split("=")]
                    name = tokens[0]
                    value = tokens[1]
                    variables[name] = value
                elif line == "":
                    continue

        # Replace variables
        for key, value in variables.items():
            style = style.replace(key, value)

        return style
