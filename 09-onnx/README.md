# 09 - ONNX Runtime Edge Inference

Inferencia de red neuronal corriendo en la Raspberry Pi 3B+ usando
ONNX Runtime. El mismo modelo del PFC deployado con un runtime liviano
y portable.

## Qué hace
- Convierte un modelo Keras (.h5) a formato ONNX
- Recorre la imagen con una ventana deslizante de 15x15 píxeles (1344 parches)
- Clasifica cada parche como huevo o no huevo
- Reporta el conteo total y el tiempo de inferencia

## Resultados
- full_test.bmp → 94 huevos (referencia float32: 94) ✓
- Test2.bmp     → 99 huevos (referencia float32: 101)
- Tiempo de inferencia pura: ~1.0s
- Tiempo real total: ~2s

## Conversión del modelo
```bash
python3 convert_to_onnx.py
```

Convierte el modelo Keras a ONNX con opset 13 en float32.
Los pesos no están cuantizados — por eso el modelo pesa más que
el .tflite pero el runtime es mucho más liviano.

## Inferencia
```bash
python3 inference_onnx.py
```

## Dependencias
```bash
python3 -m pip install onnxruntime tf2onnx onnx --break-system-packages
```

## Performance real

```
Métrica                    ONNX Runtime (este proyecto)    TFLite (08-tflite)
────────────────────────────────────────────────────────────────────────────
Inferencia pura            1.0s                            0.5s
Tiempo real total          2s                              14s
Overhead startup           1s                              13.5s
Tamaño modelo              192KB (float32)                 57KB (INT8w)
```

ONNX Runtime es más lento en inferencia pura porque el modelo no está
cuantizado. Pero el runtime es mucho más liviano que TensorFlow completo
— 2s totales vs 14s de TFLite para scripts puntuales.

## ONNX vs TFLite
ONNX es un formato abierto e interoperable — el mismo .onnx puede correr
con ONNX Runtime, importarse en PyTorch, o optimizarse con TensorRT u
OpenVINO. TFLite solo corre con el runtime de Google.
