import matplotlib.pyplot as plt

pixels = [
    {"red": 100, "green": 150, "blue": 200},
    {"red": 100, "green": 100, "blue": 150},
    {"red": 200, "green": 150, "blue": 100}
]

histogramared = [0] * 256
histogramagreen = [0] * 256
histogramblue = [0] * 256

for pixel in pixels:
    histogramared[pixel["red"]] += 1
    histogramagreen[pixel["green"]] += 1
    histogramblue[pixel["blue"]] += 1

plt.figure(figsize=(15, 5))

plt.subplot(1, 3, 1)
plt.bar(range(256), histogramared, color='red', alpha=0.7, width=10)
plt.title('Histograma Vermelho')
plt.xlabel('Intensidade')
plt.ylabel('Frequência')

plt.subplot(1, 3, 2)
plt.bar(range(256), histogramagreen, color='green', alpha=0.7, width=10)
plt.title('Histograma Verde')
plt.xlabel('Intensidade')
plt.ylabel('Frequência')

plt.subplot(1, 3, 3)
plt.bar(range(256), histogramblue, color='blue', alpha=0.7, width=10)
plt.title('Histograma Azul')
plt.xlabel('Intensidade')
plt.ylabel('Frequência')

plt.tight_layout()
plt.show()
