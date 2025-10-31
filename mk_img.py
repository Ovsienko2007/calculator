import cv2
import sys

default_video_name       = "video.mp4"
default_code_file_name   = "video.asm"

byte_size              = 256
space_ANCII            = 32
default_x_size         = 210
default_y_size         = 60

def check_args():
    err = False
    args = [default_video_name, default_code_file_name]

    if len(sys.argv) <= 3:
        for arg_ind in range(1, len(sys.argv)):
            args[arg_ind - 1] = str(sys.argv[arg_ind])
    else:
        err = True
        print("Error: incoret arguments number")
    
    return err, args


def loading_displaying_saving_enhanced(video_name, code_file_name):
    err = False

    with open(code_file_name, 'w') as file:
        cap = cv2.VideoCapture(video_name)
        
        while True:
            ret, img = cap.read()
            if not ret:
                break

            new_x_size = 210
            new_y_size = int(img.shape[0] * (210 / img.shape[1]) // 2)
            Contrast  = 1.2
            luminance = 10
            
            resized_img = cv2.resize(img, (new_x_size, new_y_size), interpolation=cv2.INTER_AREA)
            enhanced_img = cv2.convertScaleAbs(resized_img, alpha=Contrast, beta=luminance)

            for y_pos in range(default_y_size):
                for x_pos in range(enhanced_img.shape[1]):
                    if y_pos < enhanced_img.shape[0]:
                        (b, g, r) = enhanced_img[y_pos, x_pos]
                        new_elem = int(r) * byte_size ** 3 + int(g) * byte_size ** 2 + int(b) * byte_size + space_ANCII # TODO ANSI?
                        file.write(f"PUSH {new_elem}\n"
                                    "POP [RAX]\n"
                                    "PUSH 1\n"
                                    "PUSH RAX\n"
                                    "ADD\n"
                                    "POP RAX\n")

            file.write(f"CLEAR\n"
                        "SHOW\n"
                        "PUSH 0\n"
                        "POP RAX\n")
        file.write(f"HALT\n")
    return False

def main():
    err = False

    err, [video_name, code_file_name] = check_args()

    if not err:
        err = loading_displaying_saving_enhanced(video_name, code_file_name)
    if err:
        print("Image processing was not completed")


if __name__ == "__main__":
    main()