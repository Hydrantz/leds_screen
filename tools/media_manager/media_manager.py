import imageio.v3 as imageio, numpy as np

def load_img(path: str):
    img = imageio.imread("in.jpg")
    return img
    
def neopixel_formatter(img: np.ndarray):
    formatted_img = img.copy()
    formatted_img[:, 1::2] = formatted_img[::-1, 1::2]
    formatted_img = np.transpose(formatted_img, (1,0,2))
    return formatted_img

def write_file(img: np.ndarray, path: str):
    f = open(path, "wb")
    f.write(img.tobytes())
    f.close()

def img2neopixel(img_path: str, bin_path: str):
    write_file(neopixel_formatter(load_img(img_path)),
               bin_path)
    
    
def neopixel2img(bin_path: str, height: int, width: int):
    img = np.fromfile(bin_path, dtype="uint8")
    img = np.reshape(img, (height, width, 3))
    img = np.transpose(img, (1,0,2))
    img[:, 1::2] = img[::-1, 1::2]
    return img