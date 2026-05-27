import openvino as ov
import numpy as np
import os

MODEL_PATH  = "small_gray.onnx"
OUTPUT_DIR  = "."

print("Cargando modelo ONNX...")
core = ov.Core()

print("Convirtiendo a IR (Intermediate Representation)...")
model = ov.convert_model(MODEL_PATH)

print("Guardando IR...")
ov.save_model(model, "small_gray.xml")

print("\nArchivos generados:")
print(f"  small_gray.xml: {os.path.getsize('small_gray.xml') / 1024:.1f} KB")
print(f"  small_gray.bin: {os.path.getsize('small_gray.bin') / 1024:.1f} KB")

print("\nInspeccionando modelo...")
for input in model.inputs:
    print(f"Input:  {input.any_name} shape={input.shape} dtype={input.element_type}")
for output in model.outputs:
    print(f"Output: {output.any_name} shape={output.shape} dtype={output.element_type}")
