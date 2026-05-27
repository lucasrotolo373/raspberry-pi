import numpy as np
import openvino as ov
from PIL import Image
from time import time

MODEL_XML  = "small_gray.xml"
IMAGE_PATH = "full_test.bmp"
PATCH_SIZE = 15
THRESH     = 0.9

print("Cargando modelo...")
core           = ov.Core()
model          = core.read_model(MODEL_XML)
compiled_model = core.compile_model(model, "CPU")

input_layer  = compiled_model.input(0)
output_layer = compiled_model.output(0)

print(f"Input:  {input_layer.any_name} shape={input_layer.shape}")
print(f"Output: {output_layer.any_name} shape={output_layer.shape}")
print(f"Device: CPU")

img       = Image.open(IMAGE_PATH).convert('L')
img_array = np.array(img, dtype=np.float32) / 255.0
h, w      = img_array.shape
print(f"Imagen: {w}x{h} px")

resultados = []
start = time()

for j in range(h // PATCH_SIZE):
    for i in range(w // PATCH_SIZE):
        patch = img_array[j*15:j*15+15, i*15:i*15+15].reshape(1, 15, 15, 1)

        result = compiled_model({input_layer: patch})
        if j == 0 and i == 0:
            print(f"output raw: {result[output_layer][0]}")
        res    = result[output_layer][0]

        if res[0] > 0.3:
            res[1] = 0
        resultados.append(res[1])

stop = time()

huevos = sum(r > THRESH for r in resultados)
import collections
rangos = collections.Counter()
for r in resultados:
    if r > 0.1:
        bucket = round(float(r), 1)
        rangos[bucket] += 1
print("Distribución de probabilidades > 0.1:")
for k in sorted(rangos.keys()):
    print(f"  {k:.1f}: {rangos[k]} parches")
print(f"\nHuevos detectados: {huevos}")
print(f"Tiempo de inferencia: {stop - start:.4f}s")
