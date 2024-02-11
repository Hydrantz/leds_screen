import imageio.v3 as imageio, numpy as np, matplotlib.pyplot as plt

yiq_from_rgb = np.array([[0.299,      0.587,        0.114],
                                 [0.59590059, -0.27455667, -0.32134392],
                                 [0.21153661, -0.52273617, 0.31119955]])

def load_img(path: str):
    img = imageio.imread(path)
    show_img(img)
    return img
    
def neopixel_formatter(img: np.ndarray):
    new_im = normalize_brightness(img)
    show_img(new_im)
    formatted_img = np.zeros_like(new_im)
    formatted_img[:,:,0] = new_im[:,:,1]
    formatted_img[:,:,1] = new_im[:,:,0]
    formatted_img[:,:,2] = new_im[:,:,2]
    show_img(formatted_img)
    formatted_img[:, 1::2] = formatted_img[::-1, 1::2]
    show_img(formatted_img)
    formatted_img = np.transpose(formatted_img, (1,0,2))
    show_img(formatted_img)
    return formatted_img

def write_file(img: np.ndarray, path: str):
    f = open(path, "wb")
    f.write(img.tobytes())
    f.close()

def normalize_brightness(img: np.ndarray):
    # yiq = np.dot(img, yiq_from_rgb.T.copy())
    # new_img = yiq[:,:,0]**3
    # new_img = (new_img/new_img.max()*255).astype("uint8")
    # show_img(new_img)
    # yiq[:,:,0] = new_img
    # fin = np.dot(yiq, np.linalg.inv(yiq_from_rgb)).astype("uint8")
    fin = img.copy()
    fin[fin<20] = 0
    return fin



def img2neopixel(img_path: str, bin_path: str):
    write_file(neopixel_formatter(load_img(img_path)),
               bin_path)
    
    
def neopixel2img(bin_path: str, height: int, width: int):
    img = np.fromfile(bin_path, dtype="uint8")
    img = np.reshape(img, (height, width, 3))
    img = np.transpose(img, (1,0,2))
    img[:, 1::2] = img[::-1, 1::2]
    formatted_img = np.zeros_like(img)
    formatted_img[:,:,0] = img[:,:,1]
    formatted_img[:,:,1] = img[:,:,0]
    formatted_img[:,:,2] = img[:,:,2]
    return formatted_img

def load_neopixel(bin_path: str):
    f = open(bin_path, "rb")
    return bytearray(f.read())

def show_img(img):
    plt.imshow(img, cmap="gray")
    plt.show()