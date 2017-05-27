#!/usr/bin/env python

import subprocess, sys, os

platform = sys.argv[1]
project = sys.argv[2]

def exe_cmd(cmd):
  subprocess.call(cmd, shell=True)

def get_raw_img_path():
  for parent,dirnames,filenames in os.walk("release"): 
    if project in parent:
      print parent
      return parent

def get_pl_raw_img(raw_img_dir):
  for parent,dirnames,filenames in os.walk(raw_img_dir): 
    for f in filenames:
      if "preloader_" in f:
        return os.path.join(parent, f)

def sign_pl(raw_pl_img):
  pl_img = raw_pl_img
  key_encode_tool = "./gionee/config/tools/verified_boot_tools/KeyEncode"
  key_coded_pl = pl_img
  pbp_tool = "gionee/config/tools/verified_boot_tools/PBP_PL"
  CHIP_CONFIG = "./gionee/config/tools/verified_boot_tools/configs/CHIP_CONFIG.ini"
  CHIP_KEY = "./gionee/config/tools/verified_boot_tools/configs/CHIP_TEST_KEY.ini"
  GFH_PATH = "./gionee/config/tools/verified_boot_tools/configs/GFH_CONFIG.ini"
  if not os.path.exists(pl_img):
    return
  exe_cmd("{} {} {}".format(key_encode_tool, pl_img, key_coded_pl))
  exe_cmd("{} -m {} -i {} -g {} {}".format(pbp_tool, CHIP_CONFIG, CHIP_KEY, GFH_PATH, key_coded_pl))

def save_signed_img(dst_path):
  exe_cmd("mv release/tmp/*-verified* {}".format(dst_path))

def main():
  raw_img_path = get_raw_img_path()
  raw_pl_path = get_pl_raw_img(raw_img_path)
  sign_pl(raw_pl_path)
  exe_cmd("./gionee/config/tools/verified_boot_tools/SignFlow.py {} {} > {}".format(platform, raw_img_path, "release/sign_log"))
  save_signed_img(raw_img_path)

if __name__ == '__main__':
  main()
