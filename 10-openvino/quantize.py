import numpy as np
import openvino as ov
import nncf

MODEL_PATH  = "small_gray.xml"
CALIB_PATH  = "calib_data_grayscale.npy"
OUTPUT_PATH = "small_gray_int8.xml"

print("Cargando modelo IR...")
core  = ov.Core()
model = core.read_model(MODEL_PATH)

print("Cargando datos de calibración...")
calib_data = np.load(CALIB_PATH)
print(f"Shape: {calib_data.shape}  dtype: {calib_data.dtype}")

def calibration_dataset():
    for i in range(len(calib_data)):
        sample = calib_data[i].astype(np.float32)
        sample = np.expand_dims(sample, axis=0)
        yield {"args_0": sample}

print("Cuantizando a INT8...")
quantized_model = nncf.quantize(
    model,
    calibration_dataset=nncf.Dataset(calibration_dataset()),
    preset=nncf.QuantizationPreset.MIXED,
)

print("Guardando modelo cuantizado...")
ov.save_model(quantized_model, OUTPUT_PATH)

import os
print(f"\nTamaño IR float32: {os.path.getsize('small_gray.bin') / 1024:.1f} KB")
print(f"Tamaño IR INT8:    {os.path.getsize('small_gray_int8.bin') / 1024:.1f} KB")
