#!/usr/bin/env python3

import optparse, os, sys, shutil, logging, time, subprocess

LOG_DIR = os.path.join(os.getcwd(), 'release/')
log_file = os.path.join(LOG_DIR, 'log-' + time.strftime('%Y-%m-%d-%H-%M-%S',time.localtime(time.time())))
KEYS = {"oem_img_key": "vendor/mediatek/proprietary/custom/gionee6795_lwt_l/security/image_auth/VERIFIED_BOOT_IMG_AUTH_KEY.ini", 
        "da_pl_key": "vendor/mediatek/proprietary/custom/gionee6795_lwt_l/security/image_auth/IMG_AUTH_KEY.ini",
        "da_pl_key_cfg": "vendor/mediatek/proprietary/custom/gionee6795_lwt_l/security/image_auth/IMG_AUTH_CFG.ini"}
TOOLS = {"rsa1024": "vendor/mediatek/proprietary/scripts/sign-image/SignTool.sh",
         "rsa2048": "vendor/mediatek/proprietary/scripts/sign-image/SignTool_2048",
         "sign_tool": "vendor/mediatek/proprietary/scripts/sign-image/SignTool",
         "Simg2imgTool": "vendor/mediatek/proprietary/scripts/sign-image/simg2img"}
VERIFY_BOOT_IMAGES = ('lk.bin', 'logo.bin', 'secro.img', 'boot.img', 'recovery.img')
SECURE_DOWNLOAD_IMAGES = ('lk.bin', 'logo.bin', 'secro.img', 'boot.img', 'recovery.img', 'system.img', 'userdata.img', 'trustzone.bin', 'tz.img', 'mobicore.bin', 'cache.img', 'custom.img', 'MBR', 'EBR1', 'EBR2')
SIGN_PROJECT = ('GBL5801A02', )

def is_opt_exist(opt_item, opt_key, opt_dict):
  if not opt_item:
    logging.error("please check out input arguement: {}, exit".format(opt_key))
    sys.exit(1)
  opt_dict[opt_key] = opt_item

def parse_options():
  opt_dict = {}
  usage = "usage: %prog [options] arg"  
  parser = optparse.OptionParser(usage)  
  parser.add_option("-k", "--keys", dest="keys_dir", help="gionee keys directory")
  parser.add_option("-t", "--tools", dest="tools_dir", help="tools directory")
  parser.add_option("-s", "--src_img", dest="src_img_dir", help="source images directory")
  parser.add_option("-d", "--dst_img", dest="dst_img_dir", help="destination images directory")
  (options, args) = parser.parse_args()

  is_opt_exist(options.src_img_dir, "src_img_dir", opt_dict)
  is_opt_exist(options.keys_dir, "keys_dir", opt_dict)
  is_opt_exist(options.tools_dir, "tools_dir", opt_dict)
  is_opt_exist(options.dst_img_dir, "dst_img_dir", opt_dict)

  for k in opt_dict.keys():
    logging.info("{} is: {}".format(k, opt_dict[k]))

  return opt_dict

def creat_log_dir():
  if not os.path.exists(LOG_DIR):
    os.makedirs(LOG_DIR)

def get_keys_dir(dir_dict):
  return dir_dict['keys_dir']

def get_src_img_dir(dir_dict):
  return dir_dict['src_img_dir']

def get_dst_img_top_dir(dir_dict):
  return dir_dict['dst_img_dir']

def get_tools_dir(dir_dict):
  return dir_dict['tools_dir']

def get_img_version_num(dir_dict):
  src_img_dir = get_src_img_dir(dir_dict)
  return os.path.split(src_img_dir)[1]

def get_signed_img_dir(dir_dict):
  img_version = get_img_version_num(dir_dict)
  dst_img_top_dir = get_dst_img_top_dir(dir_dict)
  signed_img_dir = os.path.join(dst_img_top_dir, img_version)
  return signed_img_dir

def mk_signed_img_dir(dir_dict):
  signed_img_dir = get_signed_img_dir(dir_dict)
  if os.path.exists(signed_img_dir):
    shutil.rmtree(signed_img_dir)
  os.makedirs(signed_img_dir)

def get_sign_tmp_dir(dir_dict):
  img_version = get_img_version_num(dir_dict)
  dst_img_top_dir = get_dst_img_top_dir(dir_dict)
  sign_tmp_dir = os.path.join(dst_img_top_dir, img_version + "tmp")
  return sign_tmp_dir

def mk_sign_tmp_dir(dir_dict):
  sign_tmp_dir = get_sign_tmp_dir(dir_dict)
  if os.path.exists(sign_tmp_dir):
    shutil.rmtree(sign_tmp_dir)
  os.makedirs(sign_tmp_dir)

def is_nessary_file_exist(path_name, file_name):
  file_full_path = os.path.join(path_name, file_name)
  if not os.path.exists(file_full_path):
    logging.error("{} does not exist, exit".format(file_full_path))
    sys.exit(1)

def have_to_sign(dir_dict):
  version_num = get_img_version_num(dir_dict)
  for proj in SIGN_PROJECT:
    if version_num.startswith(proj):
       logging.info("have to sign {}".format(version_num))
       return

  sign_img_dir = get_dst_img_top_dir(dir_dict)
  if os.path.exists(sign_img_dir):
    shutil.rmtree(sign_img_dir)
  logging.error("{} do not have to sign, exit".format(version_num))
  sys.exit(1)

def check_tools(dir_dict):
  tool_path = get_tools_dir(dir_dict)
  for k in TOOLS:
    is_nessary_file_exist(tool_path, TOOLS[k])

def check_keys(dir_dict):
  keys_path = get_keys_dir(dir_dict)
  for k in KEYS:
    is_nessary_file_exist(keys_path, KEYS[k])

def setting_check(dir_dict):
  have_to_sign(dir_dict)
  check_tools(dir_dict)
  check_keys
  logging.info("settings check ok")

def copy_file(src_file, dst_dir):
  subprocess.call(["cp", src_file, dst_dir])

def save_boot_signiture(dir_dict):
  dst_dir = os.path.join('release', 'sec_boot_sig_dir')
  src_dir = get_sign_tmp_dir(dir_dict)
  subprocess.call(["mkdir", "-p", dst_dir])
  for f in ("boot.img.signiture", "recovery.img.signiture"):
    src_file = os.path.join(src_dir, f)
    dst_file = os.path.join(dst_dir, f.split(".")[0]+".sig")
    subprocess.call(["cp", src_file, dst_file])

def move_signed_image_to_relase_dir(dir_dict):
  signed_img_dir = get_signed_img_dir(dir_dict)
  src_img_dir = get_src_img_dir(dir_dict)
  for parent,dirnames,filenames in os.walk(signed_img_dir): 
    for f in filenames:
      src_file = os.path.join(parent, f)
      logging.info("copy {} to {}".format(src_file, src_img_dir))
      copy_file(src_file, src_img_dir)

def remove_tmp(dir_dict):
  sign_tmp_dir = get_sign_tmp_dir(dir_dict)
  if os.path.exists(sign_tmp_dir):
    shutil.rmtree(sign_tmp_dir)

def setting_init(dir_dict):
  mk_signed_img_dir(dir_dict)
  mk_sign_tmp_dir(dir_dict)
  logging.info("settings init ok")

def sign_pl():
  pass

def sign_for_verify_boot(dir_dict):
  keys_dir = get_keys_dir(dir_dict)
  tools_dir = get_tools_dir(dir_dict)
  src_img_dir = get_src_img_dir(dir_dict)
  sign_img_tmp_dir = get_sign_tmp_dir(dir_dict)

  signtool_rsa2048 = os.path.join(tools_dir, TOOLS['rsa2048'])
  oem_img_key = os.path.join(keys_dir, KEYS['oem_img_key'])

  for img in VERIFY_BOOT_IMAGES:
    orig_img = os.path.join(src_img_dir, img)
    verified_img = os.path.join(sign_img_tmp_dir, img + ".verified")
    signiture = os.path.join(sign_img_tmp_dir, img + ".signiture")
    subprocess.call([signtool_rsa2048, oem_img_key, orig_img, verified_img, signiture], stdout=open(log_file, 'a'), stderr=open(log_file, 'a'))

def sign_for_secure_download(dir_dict):
  tools_dir = get_tools_dir(dir_dict)
  keys_dir = get_keys_dir(dir_dict)
  src_img_dir = get_src_img_dir(dir_dict)
  sign_img_tmp_dir = get_sign_tmp_dir(dir_dict)
  signed_img_dir = get_signed_img_dir(dir_dict)

  signtool_rsa1024 = os.path.join(tools_dir, TOOLS['rsa1024'])
  da_pl_key = os.path.join(keys_dir, KEYS['da_pl_key'])
  da_pl_key_cfg = os.path.join(keys_dir, KEYS['da_pl_key_cfg'])

  for img in SECURE_DOWNLOAD_IMAGES:
    orig_img = os.path.join(src_img_dir, img)
    verified_img = os.path.join(sign_img_tmp_dir, img + ".verified")
    signed_img = os.path.join(signed_img_dir, img)
    if os.path.exists(orig_img):
      if os.path.exists(verified_img):
        subprocess.call([signtool_rsa1024, da_pl_key, da_pl_key_cfg, verified_img, signed_img], stdout=open(log_file, 'a'), stderr=open(log_file, 'a'))
      else:
        subprocess.call([signtool_rsa1024, da_pl_key, da_pl_key_cfg, orig_img, signed_img], stdout=open(log_file, 'a'), stderr=open(log_file, 'a'))

def sign_images(dir_dict):
  sign_pl()
  sign_for_verify_boot(dir_dict)
  save_boot_signiture(dir_dict)
  sign_for_secure_download(dir_dict)

def main():  
  creat_log_dir()
  logging.basicConfig(level=logging.DEBUG,
                format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                datefmt='%a, %d %b %Y %H:%M:%S',
                filename=log_file,
                filemode='w')
  dir_dict = parse_options()
  setting_init(dir_dict)
  setting_check(dir_dict)
  sign_images(dir_dict)
  remove_tmp(dir_dict)
  move_signed_image_to_relase_dir(dir_dict)
  
if __name__ == '__main__':
  main()
