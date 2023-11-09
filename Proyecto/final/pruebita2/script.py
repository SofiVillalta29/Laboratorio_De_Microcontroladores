import serial
import re

# Inicia conexión serial
ser = serial.Serial('/dev/ttyACM0', 115200)

# Expresión regular para capturar las etiquetas y valores de predicción
pattern = re.compile(r"(\w+): ([0-9]+\.[0-9]+)")

# Lista de notas afinadas permitidas
allowed_tuned_notes = {"tuneda", "tunedd", "tunedb", "tunede", "tunede2"}

# Espera por datos
print("Esperando datos...")

# Procesa continuamente las líneas que vienen del serial
while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()

        # Busca coincidencias en la línea actual
        matches = pattern.findall(line)
        highest_prob = 0.0
        best_tuned_note = None

        for match in matches:
            label, value = match
            probability = float(value)

            # Verifica si la etiqueta es una de las notas afinadas permitidas y la probabilidad es mayor a 0.85
            if label in allowed_tuned_notes and probability > highest_prob and probability > 0.85:
                highest_prob = probability
                best_tuned_note = label

        # Imprime la nota afinada si se encontró alguna con probabilidad mayor a 0.85
        if best_tuned_note:
            print(f"Nota afinada detectada: {best_tuned_note} con probabilidad {highest_prob}")

        # Reinicia las variables para la próxima iteración
        highest_prob = 0.0
        best_tuned_note = None
