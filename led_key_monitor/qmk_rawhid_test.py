import hid
import time
import cv2
import numpy as np
import itertools

# HIDを列挙
def enum_hid():
    # Check all  usb device
    for d in hid.enumerate(0, 0):
        keys = d.keys()
        #keys.sort()
        for key in keys:
            print ("%s : %s" % (key, d[key]))
        print ("")

# HIDを検索してパスをもらう
def findHIDPath(vid, pid, usagepage, usage):
    # Check all  usb device
    for d in hid.enumerate(0, 0):
        keys = d.keys()
        if (d['vendor_id'] == vid) and (d['product_id'] == pid) and (d['usage_page'] == usagepage) and (d['usage'] == usage):
            for key in keys:
                print ("%s : %s" % (key, d[key]))
            return d['path']



mat_W_size = 128
mat_H_size = 128

mat_W_size = 23
mat_H_size =  6

led_num = 63 + 35
byte_per_send = 30

def hsv_to_rgb(h, s, v):
    bgr = cv2.cvtColor(np.array([[[h, s, v]]], dtype=np.uint8), cv2.COLOR_HSV2BGR)[0][0]
    return (bgr[2], bgr[1], bgr[0])

def gen_pattern(cnt):
    cnt *= 5
    cnt %= 256
    img = np.zeros((mat_H_size, mat_W_size, 3), np.uint8)
    for y in range(mat_H_size):
        for x in range(mat_W_size):
            img[y, x] = [x*255/mat_W_size, y*255/mat_H_size, cnt]
    img = np.flipud(img)
    return img

def gen_solid_mat(cnt):
    cnt %= 240
    p = hsv_to_rgb(cnt, 255, 255)
    img = np.full((mat_H_size, mat_W_size, 3), p)
    return img

def gen_pattern3(cnt):
    cnt %= 360
    img = np.linspace([cnt, 1.0,1.0], [cnt+360, 1.0,1.0], mat_W_size*mat_H_size, dtype=np.float32).reshape(mat_H_size, mat_W_size, 3)
    img = cv2.cvtColor(img, cv2.COLOR_HSV2BGR)
    img *= 255
    img = img.astype(np.uint8)
    return img

def gen_pattern_gray(cnt):
    cnt *= 5
    cnt %= 256
    img = np.zeros((mat_H_size, mat_W_size, 3), np.uint8)
    for y in range(mat_H_size):
        for x in range(mat_W_size):
            v = (x*255/mat_W_size + cnt)%256
            img[y, x] = [v, v, v]
    return img


def sample_lin(img):
    h = img.shape[0]
    w = img.shape[1]
    s = np.arange(led_num)
    s = img[s*h//led_num, s*w//led_num]
    return s

def sample(img, key_points):
    s = img[key_points[0], key_points[1]]
    # s = img[key_points]
    return s

def led_encode(s):
    e = np.arange(led_num)
    e = s[e, 0]//43 + \
        s[e, 1]//43 * 6 + \
        s[e, 2]//43 * 36
    # e = s[e, 0]//(256//4) * 64 + \
    #     s[e, 1]//(256//4) *  2 + \
    #     s[e, 2]//(256//4) * 16
    # e = s[e, 0]//(256//4) * 64 + \
        # s[e, 1]//(256//8) *  1 + \
        # s[e, 2]//(256//8) *  8
    return e

def led_encode_gray(s):
    e = np.arange((led_num+1)//2) * 2
    va = (s[e+0, 0])//(256//16)
    vb = (s[e+1, 0])//(256//16)
    e = va + vb * 16
    return e

def send_led_impl(h, led_dat, line, isGray):
    send_buf = [0] * 33
    send_buf[1] = line + (isGray * 128)
    j = line*byte_per_send
    for i in range(byte_per_send):
        if j < len(led_dat):
            send_buf[i+2] = led_dat[j]
            j+=1
    # print(send_buf)
    h.write(send_buf)

def send_led(h, led_dat):
    if len(led_dat) == led_num:
        for line in range((len(led_dat)+byte_per_send-1)//byte_per_send):
            send_led_impl(h, led_dat, line, 0)
    else:
        for line in range((len(led_dat)+byte_per_send-1)//byte_per_send):
            send_led_impl(h, led_dat, line, 1)


def make_key_points_pair(km):
    s = np.zeros(led_num, np.uint8)
    sum_y = np.zeros(led_num, np.int32)
    cnt_y = np.zeros(led_num, np.int32)
    sum_x = np.zeros(led_num, np.int32)
    cnt_x = np.zeros(led_num, np.int32)
    for y in range(km.shape[0]-1,-1,-1):
        for x in range(km.shape[1]):
            if km[y, x] == 0 : continue
            sum_y[km[y, x]-1] += y
            cnt_y[km[y, x]-1] += 1
            sum_x[km[y, x]-1] += x
            cnt_x[km[y, x]-1] += 1
    # res = np.stack([sum_y*8//cnt_y, sum_x*2//cnt_x], 1)
    res = np.stack([sum_y*8//cnt_y, sum_x*2//cnt_x])
    return res

keymap_wide = np.array([
   [35,	35,	35,	35,	34,	34,	34,	34,	33,	33,	33,	33,	32,	32,	32,	32,	31,	31,	31,	31,	30,	30,	30,	30,	29,	29,	29,	29,	0,	0,	0,	0,	0,	0,	0,	0,	36,	36,	36,	36,	37,	37,	37,	37,	38,	38,	38,	38,	39,	39,	39,	39,	40,	40,	40,	40,	41,	41,	41,	41,	42,	42,	42,	42,	43,	43,	43,	43,	44,	44,	44,	44,	0,	0,	0,	0,	95,	95,	95,	95,	96,	96,	96,	96,	97,	97,	97,	97,	98,	98,	98,	98],
   [22,	22,	22,	22,	23,	23,	23,	23,	24,	24,	24,	24,	25,	25,	25,	25,	26,	26,	26,	26,	27,	27,	27,	27,	28,	28,	28,	28,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	52,	52,	52,	52,	51,	51,	51,	51,	50,	50,	50,	50,	49,	49,	49,	49,	48,	48,	48,	48,	47,	47,	47,	47,	46,	46,	46,	46,	45,	45,	45,	45,	45,	45,	0,	0,	0,	0,	94,	94,	94,	94,	93,	93,	93,	93,	92,	92,	92,	92,	91,	91,	91,	91],
   [21,	21,	21,	21,	21,	20,	20,	20,	20,	19,	19,	19,	19,	18,	18,	18,	18,	17,	17,	17,	17,	16,	16,	16,	16,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	53,	53,	53,	53,	54,	54,	54,	54,	55,	55,	55,	55,	56,	56,	56,	56,	57,	57,	57,	57,	58,	58,	58,	58,	59,	59,	59,	59,	60,	60,	60,	60,	45,	45,	45,	45,	0,	0,	0,	0,	87,	87,	87,	87,	88,	88,	88,	88,	89,	89,	89,	89,	90,	90,	90,	90],
   [ 9,	9,	9,	9,	9,	9,	10,	10,	10,	10,	11,	11,	11,	11,	12,	12,	12,	12,	13,	13,	13,	13,	14,	14,	14,	14,	15,	15,	15,	15,	0,	0,	0,	0,	0,	0,	0,	0,	68,	68,	68,	68,	67,	67,	67,	67,	66,	66,	66,	66,	65,	65,	65,	65,	64,	64,	64,	64,	63,	63,	63,	63,	62,	62,	62,	62,	61,	61,	61,	61,	61,	61,	0,	0,	0,	0,	86,	86,	86,	86,	85,	85,	85,	85,	84,	84,	84,	84,	83,	83,	83,	83],
   [ 8,	8,	8,	8,	7,	7,	7,	7,	6,	6,	6,	6,	5,	5,	5,	5,	4,	4,	4,	4,	3,	3,	3,	3,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	69,	69,	69,	69,	70,	70,	70,	70,	71,	71,	71,	71,	71,	71,	72,	72,	72,	72,	73,	73,	73,	73,	74,	74,	74,	74,	74,	75,	75,	75,	75,	76,	76,	76,	76,	0,	0,	80,	80,	80,	80,	0,	0,	81,	81,	81,	81,	81,	81,	82,	82,	82,	82,	83,	83,	83,	83],
   [ 0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	2,	2,	2,	2,	1,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	77,	77,	77,	78,	78,	78,	78,	79,	79,	79,	79,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0]
])

keymap_normal = np.array([
   [0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	1,	2,	2,	2,	2,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0],
   [0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	3,	3,	3,	3,	4,	4,	4,	4,	5,	5,	5,	5,	6,	6,	6,	6,	7,	7,	7,	7,	8,	8,	8,	8,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0],
   [0,	0,	0,	0,	0,	0,	0,	15,	15,	15,	15,	14,	14,	14,	14,	13,	13,	13,	13,	12,	12,	12,	12,	11,	11,	11,	11,	10,	10,	10,	10,	9,	9,	9,	9,	9,	9,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0],
   [0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	16,	16,	16,	16,	17,	17,	17,	17,	18,	18,	18,	18,	19,	19,	19,	19,	20,	20,	20,	20,	21,	21,	21,	21,	21,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0],
   [0,	0,	0,	0,	0,	0,	0,	0,	0,	28,	28,	28,	28,	27,	27,	27,	27,	26,	26,	26,	26,	25,	25,	25,	25,	24,	24,	24,	24,	23,	23,	23,	23,	22,	22,	22,	22,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0],
   [0,	0,	0,	0,	0,	0,	0,	0,	0,	29,	29,	29,	29,	30,	30,	30,	30,	31,	31,	31,	31,	32,	32,	32,	32,	33,	33,	33,	33,	34,	34,	34,	34,	35,	35,	35,	35,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0],
   [0,	36,	36,	36,	36,	37,	37,	37,	37,	38,	38,	38,	38,	39,	39,	39,	39,	40,	40,	40,	40,	41,	41,	41,	41,	42,	42,	42,	42,	43,	43,	43,	43,	44,	44,	44,	44,	0,	0,	0,	0,	95,	95,	95,	95,	96,	96,	96,	96,	97,	97,	97,	97,	98,	98,	98,	98],
   [0,	0,	0,	52,	52,	52,	52,	51,	51,	51,	51,	50,	50,	50,	50,	49,	49,	49,	49,	48,	48,	48,	48,	47,	47,	47,	47,	46,	46,	46,	46,	45,	45,	45,	45,	45,	45,	0,	0,	0,	0,	94,	94,	94,	94,	93,	93,	93,	93,	92,	92,	92,	92,	91,	91,	91,	91],
   [0,	53,	53,	53,	53,	54,	54,	54,	54,	55,	55,	55,	55,	56,	56,	56,	56,	57,	57,	57,	57,	58,	58,	58,	58,	59,	59,	59,	59,	60,	60,	60,	60,	45,	45,	45,	45,	0,	0,	0,	0,	87,	87,	87,	87,	88,	88,	88,	88,	89,	89,	89,	89,	90,	90,	90,	90],
   [0,	0,	0,	68,	68,	68,	68,	67,	67,	67,	67,	66,	66,	66,	66,	65,	65,	65,	65,	64,	64,	64,	64,	63,	63,	63,	63,	62,	62,	62,	62,	61,	61,	61,	61,	61,	61,	0,	0,	0,	0,	86,	86,	86,	86,	85,	85,	85,	85,	84,	84,	84,	84,	83,	83,	83,	83],
   [69,	69,	69,	69,	70,	70,	70,	70,	71,	71,	71,	71,	71,	71,	72,	72,	72,	72,	73,	73,	73,	73,	74,	74,	74,	74,	74,	75,	75,	75,	75,	76,	76,	76,	76,	0,	0,	80,	80,	80,	80,	0,	0,	81,	81,	81,	81,	81,	81,	82,	82,	82,	82,	83,	83,	83,	83],
   [0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	77,	77,	77,	77,	78,	78,	78,	78,	79,	79,	79,	79,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0]
])

keymap = keymap_wide
# keymap = keymap_normal
print(keymap.shape)

mat_H_size = keymap.shape[0] * 8
mat_W_size = keymap.shape[1] * 2
key_points = make_key_points_pair(keymap)
print(key_points)
print(mat_H_size, mat_W_size)

# exit()

video = cv2.VideoCapture("../../../../video/BadApple_wide.mp4")
# video = cv2.VideoCapture("../../../../video/EMOMOMO.mp4")
video_width       = int(video.get(cv2.CAP_PROP_FRAME_WIDTH)) # 動画の画面横幅
video_height      = int(video.get(cv2.CAP_PROP_FRAME_HEIGHT)) # 動画の画面縦幅
frame_total_count = int(video.get(cv2.CAP_PROP_FRAME_COUNT)) # 総フレーム数
frame_rate        = int(video.get(cv2.CAP_PROP_FPS)) # フレームレート(fps)
resize_width  = mat_W_size
resize_height = round(video_height * (mat_W_size / video_width))
resize_left   = 0
resize_top    = (resize_height - mat_H_size)//2

resize_width2 = round(video_width * (mat_H_size / video_height))
print(resize_width, resize_width2)
if resize_width < resize_width2:
    resize_width = resize_width2
    resize_height = mat_H_size
    resize_left   = (resize_width - mat_W_size)//2
    resize_top    = 0


print("frame_rate", frame_rate)

def gen_play_video(cnt):
    if cnt >= frame_total_count:
        return np.zeros((0, 0, 1), np.uint8)
    ret, frame = video.read() # 1フレーム読み込み
    frame = cv2.resize(frame, (resize_width, resize_height))
    if resize_top >= 0 :
        frame = frame[resize_top : resize_top+mat_H_size, resize_left : resize_left + mat_W_size]
    return frame

# for i in range(frame_total_count):
#     frame = gen_play_video(i)
#     bairitsu = 2
#     cv2.imshow("image", frame.repeat(bairitsu, axis=0).repeat(bairitsu, axis=1)); cv2.waitKey(1)
#     cv2.waitKey(1000//frame_rate)

# Treadstone32のファームにRawHIDの機能を追加したので、そのVIDとPIDを指定
VENDOR_ID = 0xFEED
PRODUCT_ID = 0
#PRODUCT_ID = 0xDFA5

h = hid.device()
# VIDとPIDだと複数見つかるため、QMKから指示された設定を検索に使用
# → usagepage:0xFF60、usage:0x61
path = findHIDPath(VENDOR_ID, PRODUCT_ID, 0xFF60, 0x61)
# 取得した機器のパスでオープン
h.open_path(path)
h.get_input_report


start = time.time()
pret = start
for cnt_sum in itertools.count():
    fstart = time.time()
    # img = gen_pattern(cnt_sum)
    # img = gen_solid_mat(cnt_sum)
    # img = gen_pattern3(cnt_sum)
    # img = gen_pattern_gray(cnt_sum)
    img = gen_play_video(cnt_sum)
    if len(img) == 0:
        break
    s = sample(img, key_points)
    e = led_encode(s)
    # e = led_encode_gray(s)
    send_led(h, e)
    bairitsu = 4
    cv2.imshow("image", img.repeat(bairitsu, axis=0).repeat(bairitsu, axis=1)); cv2.waitKey(1)
    nowt = time.time()
    d    = (nowt-fstart)
    # print(e)
    print('{:.3f} {:.3f}'.format((cnt_sum+1) / (nowt-start), 1/d))
    sleep_time = max(0,(cnt_sum+1)/frame_rate - (nowt - start)); time.sleep(sleep_time)
    pret = nowt
    # cv2.waitKey(); exit()
