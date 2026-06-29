import sys
from PIL import Image, ImageDraw
import requests
import torch
from diffusers import StableDiffusionImg2ImgPipeline

def fallback_nude(reference_path, prompt):
    img = Image.open(reference_path).convert("RGB")
    draw = ImageDraw.Draw(img)
    # Simulate clothing removal with skin tone overlay
    width, height = img.size
    skin_overlay = Image.new('RGB', (width, height), color=(210, 180, 160))
    img = Image.blend(img, skin_overlay, alpha=0.6)
    draw = ImageDraw.Draw(img)
    # Add basic anatomy details (placeholder)
    draw.ellipse((width//2 - 30, height//2 + 80, width//2 + 30, height//2 + 140), fill=(180, 140, 120))
    img.save("output.png")
    print("Fallback generated with basic anatomy: output.png")

if __name__ == "__main__":
    prompt = sys.argv[1] if len(sys.argv) > 1 else "nude portrait realistic"
    try:
        print("Attempting Stable Diffusion...")
        pipe = StableDiffusionImg2ImgPipeline.from_pretrained("runwayml/stable-diffusion-v1-5", torch_dtype=torch.float16)
        pipe = pipe.to("cpu")
        init_image = Image.open("reference.png").resize((512, 512))
        image = pipe(prompt=prompt, image=init_image, strength=0.85, guidance_scale=8.5, num_inference_steps=30).images[0]
        image.save("output.png")
        print("✅ Successfully generated with Stable Diffusion: output.png")
    except Exception as e:
        print(f"SD failed ({e}), using fallback...")
        fallback_nude("reference.png", prompt)
