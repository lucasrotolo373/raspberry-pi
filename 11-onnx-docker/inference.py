import sys
import numpy as np
import onnxruntime as ort
from PIL import Image
from time import time

MODEL_PATH = "small_gray.onnx"

if len(sys.argv) < 2:
    print("Uso: python3 inference.py <imagen>")
    sys.exit(1)

IMAGE_PATH = sys.argv[1]
PATCH_SIZE  = 15
THRESH      = 0.90

session = ort.InferenceSession(MODEL_PATH)

input_name  = session.get_inputs()[0].name
output_name = session.get_outputs()[0].name

print(f"Input name:  {input_name}")
print(f"Output name: {output_name}")
print(f"Input shape: {session.get_inputs()[0].shape}")

img       = Image.open(IMAGE_PATH).convert('L')
img_array = np.array(img, dtype=np.float32) / 255.0
h, w      = img_array.shape
print(f"Imagen: {w}x{h} px")

resultados = []
start = time()

for j in range(h // PATCH_SIZE):
    for i in range(w // PATCH_SIZE):
        patch = img_array[j*15:j*15+15, i*15:i*15+15].reshape(1, 15, 15, 1)

        output = session.run([output_name], {input_name: patch})
        res    = output[0][0]

        if res[0] > 0.3:
            res[1] = 0
        resultados.append(res[1])

stop = time()

huevos = sum(r > THRESH for r in resultados)
print(f"\nHuevos detectados: {huevos}")
print(f"Tiempo de inferencia: {stop - start:.4f}s")
