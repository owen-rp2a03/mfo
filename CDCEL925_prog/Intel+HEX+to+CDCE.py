import os
from pathlib import Path
import tkinter as tk
from tkinter import messagebox
from tkinterdnd2 import DND_FILES, TkinterDnD


# -------------------------------------------------
# Intel HEX Parser
# -------------------------------------------------
def parse_intel_hex(filepath):
    registers = {}
    upper_address = 0

    with open(filepath, "r") as f:
        for line in f:
            line = line.strip()

            if not line.startswith(":"):
                continue

            byte_count = int(line[1:3], 16)
            address = int(line[3:7], 16)
            record_type = int(line[7:9], 16)
            data = line[9:9 + byte_count * 2]

            # Extended linear address record
            if record_type == 4:
                upper_address = int(data, 16) << 16
                continue

            # End of file
            if record_type == 1:
                break

            # Data record
            if record_type == 0:
                full_address = upper_address + address

                for i in range(byte_count):
                    byte_value = int(data[i * 2:i * 2 + 2], 16)
                    registers[full_address + i] = byte_value

    return registers


# -------------------------------------------------
# Generate C Array
# -------------------------------------------------
def generate_register_array(registers):
    sorted_addresses = sorted(registers.keys())

    lines = []
    lines.append("RegisterSetting cdcel925_config[] = {")

    for addr in sorted_addresses:
        lines.append(f"  {{0x{addr:02X}, 0x{registers[addr]:02X}}},")

    # Remove trailing comma from last entry
    if lines[-1].endswith(","):
        lines[-1] = lines[-1][:-1]

    lines.append("};\n")
    lines.append("const uint8_t cdcel925_config_size =")
    lines.append("  sizeof(cdcel925_config) / sizeof(cdcel925_config[0]);")

    return "\n".join(lines)


# -------------------------------------------------
# Save to Downloads
# -------------------------------------------------
def save_output(original_file, content):
    downloads_path = Path.home() / "Downloads"
    output_name = Path(original_file).stem + ".txt"
    output_path = downloads_path / output_name

    with open(output_path, "w") as f:
        f.write(content)

    return output_path


# -------------------------------------------------
# GUI
# -------------------------------------------------
class HexConverterApp(TkinterDnD.Tk):
    def __init__(self):
        super().__init__()

        self.title("Intel HEX → CDCEL925 Register Converter")
        self.geometry("500x250")
        self.configure(bg="#f0f0f0")

        self.label = tk.Label(
            self,
            text="Drag and Drop Intel HEX File Here",
            bg="white",
            relief="ridge",
            width=50,
            height=8
        )
        self.label.pack(pady=40)

        self.label.drop_target_register(DND_FILES)
        self.label.dnd_bind("<<Drop>>", self.handle_drop)

    def handle_drop(self, event):
        filepath = event.data.strip("{}")  # handles spaces in filename

        if not filepath.lower().endswith(".hex"):
            messagebox.showerror("Error", "Please drop a .hex file.")
            return

        try:
            registers = parse_intel_hex(filepath)
            output = generate_register_array(registers)
            output_path = save_output(filepath, output)

            messagebox.showinfo(
                "Success",
                f"Conversion complete!\n\nSaved to:\n{output_path}"
            )

        except Exception as e:
            messagebox.showerror("Error", f"Failed to process file:\n{e}")


# -------------------------------------------------
# Run App
# -------------------------------------------------
if __name__ == "__main__":
    app = HexConverterApp()
    app.mainloop()