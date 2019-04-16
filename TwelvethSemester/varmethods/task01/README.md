# Varmethods task01

Deconvolution with total variation regularization

## Prepared by Vladimir Liutov

Run deconvolution:

```bash
  ./main.py noised.png kernel.png denoised.png 20
```

Install requirements:

```bash
  pip install -r requirements.txt
```

Prepare image:

```bash
  ./generate_kernel.py kernel.png 1.5
  ./add_noise.py bliss_small.png kernel.png noised.png 20
```
