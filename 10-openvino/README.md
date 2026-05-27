# 10 - OpenVINO Edge Inference

Inferencia de red neuronal en la Raspberry Pi 3B+ usando OpenVINO.
El mismo modelo del PFC deployado con el toolkit de Intel para edge AI.

## Qué hace
- Convierte el modelo ONNX al formato IR de OpenVINO (.xml + .bin)
- Cuantiza a INT8 con nncf (experimental — ver nota)
- Corre inferencia float32 en CPU con el Inference Engine
- Reporta conteo, distribución de probabilidades y tiempo

## Resultados (float32)
- full_test.bmp → 94 huevos ✓
- Tiempo de inferencia pura: ~1.9s
- Tiempo real total: ~3s

## Flujo

```
small_gray.onnx
    ↓ convert_to_ir.py
small_gray.xml + small_gray.bin   ← IR float32
    ↓ quantize.py (opcional)
small_gray_int8.xml + small_gray_int8.bin   ← IR INT8
    ↓ inference_openvino.py
resultado
```

## Conversión a IR
```bash
python3 convert_to_ir.py
```

## Cuantización INT8 (opcional)
```bash
python3 quantize.py
```

**Nota:** La cuantización INT8 con nncf comprimió las probabilidades de
salida al rango 0.7-0.8, reduciendo las detecciones de 94 a 43.
El threshold calibrado para float32 (0.90) no es válido para INT8.
Para usar INT8 habría que recalibrar el threshold con imágenes de referencia.

## Inferencia
```bash
python3 inference_openvino.py
```

## Dependencias
```bash
python3 -m pip install openvino nncf tf2onnx onnx --break-system-packages
```

## OpenVINO vs ONNX Runtime vs TFLite

```
                  OpenVINO         ONNX Runtime     TFLite
Inferencia pura   1.9s             1.0s             0.5s
Tiempo real       ~3s              ~2s              ~14s
Modelo            float32 IR       float32 ONNX     INT8w tflite
Cuantización      nncf (INT8)      onnxruntime.q    TFLiteConverter
Hardware Intel    CPU/GPU/VPU      CPU              CPU
Neural Compute    Sí (MYRIAD)      No               No
Stick 2
```

## Nota sobre la Pi

OpenVINO está diseñado para hardware Intel. En ARM (Pi 3B+) corre en
modo genérico sin las optimizaciones AVX2/AVX-512. El caso de uso real
en la Pi es con el Intel Neural Compute Stick 2 (USB) que agrega un
VPU Myriad X dedicado — inferencia en milisegundos con hardware de $100.
