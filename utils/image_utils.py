import tkinter as tk
from tkinter import filedialog
from PIL import Image

def select_image_and_convert_to_txt():
    root = tk.Tk()
    root.withdraw()  # Esconde a janela principal do tkinter

    # Abre a janela de seleção de arquivo
    file_path = filedialog.askopenfilename(filetypes=[("Imagens", "*.png;*.jpg;*.jpeg;*.bmp")])

    if file_path:
        output_path = "output.txt"
        txt_from_image_rgb(file_path, output_path)
        print(f"Imagem convertida para texto RGB em {output_path}")

def txt_from_image_rgb(image_path, output_path):
    img = Image.open(image_path)

    largura, altura = img.size
    pixels = list(img.getdata())

    with open(output_path, 'w') as file:
        file.write(f"{largura}\n")
        file.write(f"{altura}\n")
        for y in range(altura):
            for x in range(largura):
                r, g, b = pixels[y * largura + x]
                file.write(f"{r} {g} {b},")
            file.write("\n")

# Exemplo de uso:
select_image_and_convert_to_txt()
