# 11 - ONNX Runtime Docker

Inferencia de red neuronal en Raspberry Pi 3B+ empaquetada en un
container Docker. El mismo modelo del 09-onnx, dockerizado para
deployment reproducible.

## Qué hace
- Corre inferencia con ONNX Runtime dentro de un container Docker
- El modelo small_gray.onnx está embebido en la imagen
- Las imágenes de entrada se pasan por volume mount en tiempo de ejecución

## Build
```bash
docker build -t onnx-inference:1.0 .
```

## Run
```bash
docker run --rm \
  -v $(pwd):/data \
  onnx-inference:1.0 \
  python3 inference.py /data/<imagen.bmp>
```

## Resultados
- full_test.bmp → 94 huevos
- Test2.bmp     → 101 huevos
- Tiempo de inferencia: ~1.1s

## Estructura
- Dockerfile — define la imagen
- requirements.txt — dependencias Python con versiones fijas
- inference.py — script de inferencia con argumento de imagen
- small_gray.onnx — modelo embebido en la imagen Docker

## Por qué Docker
Sin Docker: instalar Python, onnxruntime, numpy, Pillow, opencv
manualmente en cada dispositivo, con riesgo de incompatibilidades.
Con Docker: un build, cualquier dispositivo ARM64 con Docker lo corre
sin configuración adicional.
