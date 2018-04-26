#!/usr/bin/env python3

import optparse, subprocess, sys, os, logging, time

LOG_RETENTION = 5 #30*24*60*60 #we are gonna save logs for a month
LOG_DIR = os.path.join(os.getenv('HOME'), 'vb_sign_log/')

def exe_cmd(cmd):
  logging.info(cmd)
  subprocess.call(cmd, shell=True)

def error_exit(info):
  logging.error("error: {}, exit".format(info))
  print("\033[1;31;40m error: {}, exit \033[0m".format(info))
  sys.exit(1)

class Log:
  def __init__(self, opts_obj):
    self.log_dir = LOG_DIR
    self.log_file = os.path.join(self.log_dir, opts_obj.sw_ver + "-" + time.strftime('%Y-%m-%d-%H-%M-%S',time.localtime(time.time())))
    self.set_log_dir()
    logging.basicConfig(level=logging.DEBUG,
                format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                datefmt='%a, %d %b %Y %H:%M:%S',
                filename=self.log_file,
                filemode='w')
    self.log_retention_time = LOG_RETENTION
    self.clean_log()

  def set_log_dir(self):
    if not os.path.exists(self.log_dir):
      os.makedirs(self.log_dir)

  def clean_log(self):
    for parent,dirnames,filenames in os.walk(self.log_dir): 
      for f in filenames:
        ff = os.path.join(parent, f)
        if time.time() > os.path.getctime(ff) + self.log_retention_time:
          logging.info("remove log file==>{}, create time of {}==>{}, cur_time==>{}".format(ff, ff, os.path.getctime(ff), time.time()))
          os.remove(ff)

class CfgFiles:
  def __init__(self, opts_obj):
    self.cfg_dir = opts_obj.cfg_dir
    self.custom_cfg_dir = os.path.join(opts_obj.simg_dir, "verified_boot_cfgfiles")
    self.resign_tool = self.get_file_path("resignTool.py")
    self.pbp_tool = self.get_file_path("PBP_PL")
    self.key_encode_tool = self.get_file_path("KeyEncode")
    self.CHIP_CONFIG = self.get_file_path("CHIP_CONFIG.ini")
    self.GFH_PATH = self.get_file_path("GFH_CONFIG.ini")
    self.signflow = self.get_file_path("SignFlow.py")

  def get_file_path(self, file_name):
    default_cfg_file = os.path.join(self.cfg_dir, file_name)
    custom_cfg_file = os.path.join(self.custom_cfg_dir, file_name)
    if os.path.exists(custom_cfg_file):
      logging.info("{}==>{}".format(file_name, custom_cfg_file))
      return custom_cfg_file
    elif os.path.exists(default_cfg_file):
      logging.info("{}==>{}".format(file_name, default_cfg_file))
      return default_cfg_file
    else:
      error_exit("There is no {}".format(file_name))

class KeyFile:
  def __init__(self, opts_obj):
    self.cert1_names = ("arm7", "boot", "lk", "md1arm7", "md1", "md3", "recovery", "atf", "dsp", "logo", "md1dsp", "md1rom", "md3rom", "tee")
    self.keys_dir = opts_obj.keys_dir
    self.sbc_prik_pem = os.path.join(self.keys_dir, "sbc_privk.pem")
    self.sbc_prik_ini = os.path.join(self.keys_dir, "sbc_privk.ini")
    logging.info("private key dir: {}".format(self.keys_dir))

class ImgFile:
  def __init__(self, opts_obj, keys_obj, cfg_obj, logs_obj):
    self.sdir = opts_obj.simg_dir
    self.ddir = opts_obj.dimg_dir
    self.set_tdir()
    self.cfg = cfg_obj
    self.key = keys_obj
    self.log = logs_obj
    logging.info("src image dir: {}".format(self.sdir))
    logging.info("dst image dir: {}".format(self.ddir))
    logging.info("tmp dir: {}".format(self.tdir))

  def set_tdir(self):
    self.tdir = self.sdir + "_tmp"
    if not os.path.exists(self.tdir):
      exe_cmd("mkdir -p {}".format(self.tdir))

  def destory_tdir(self):
    exe_cmd("rm -rf {}".format(self.tdir))

  def get_raw_pl(self):
    for parent,dirnames,filenames in os.walk(self.sdir): 
      for f in filenames:
        if "preloader_" in f:
          return os.path.join(parent, f)
    return None

  def sign_pl(self, cfg, key, log, raw_pl):
    if raw_pl is None:
      print("no pl image to sign")
      return
    signed_pl = os.path.join(self.ddir, os.path.basename(raw_pl))
    exe_cmd("{} {} {} >> {}".format(cfg.key_encode_tool, raw_pl, signed_pl, log.log_file))
    exe_cmd("{} -m {} -i {} -g {} {} >> {}".format(cfg.pbp_tool, cfg.CHIP_CONFIG, key.sbc_prik_ini, cfg.GFH_PATH, signed_pl, log.log_file))

  def cert1(self, cfg, key, log):
    for img_name in key.cert1_names:
      pubk_pem = os.path.join(key.keys_dir, img_name + "_pubk.pem")
      cert1_out = os.path.join(self.tdir, img_name + "_cert1.der")
      if ((img_name == "md1rom") or (img_name == "md3rom")):
        image_path  = os.path.join(self.sdir, img_name + ".img")
        if os.path.exists(image_path):
          exe_cmd("{} type=cert1md img={} privk={} pubk={} tool={} output={} >> {}".format(cfg.resign_tool, image_path, key.sbc_prik_pem, pubk_pem, cfg.cfg_dir, self.tdir, log.log_file))
          exe_cmd("mv {}/resign/cert/cert1md/cert1md.der {}".format(self.tdir, cert1_out))
      else: 
        exe_cmd("{} type=cert1 name={} privk={} pubk={} tool={} output={} >> {}".format(cfg.resign_tool, img_name, key.sbc_prik_pem, pubk_pem, cfg.cfg_dir, self.tdir, log.log_file))
        exe_cmd("mv {}/resign/cert/cert1/{}_cert1.der {}".format(self.tdir, img_name, cert1_out))

  def cert2(self, cfg, key, log):
    exe_cmd("{} {} {} {} {} >> {}".format(cfg.signflow, key.keys_dir, self.sdir, cfg.cfg_dir, self.tdir, log.log_file))

  def save_signed_img(self):
    exe_cmd("mv {}/*-verified* {}".format(self.tdir, self.ddir))
    for parent,dirnames,filenames in os.walk(self.sdir): 
      for f in filenames:
        if os.path.exists(os.path.join(self.ddir, f)):
          continue
        org_file = os.path.join(self.sdir, f)
        exe_cmd("cp {} {}".format(org_file ,self.ddir))

  def sign(self):
    raw_pl = self.get_raw_pl()
    self.sign_pl(self.cfg, self.key, self.log, raw_pl)
    self.cert1(self.cfg, self.key, self.log)
    self.cert2(self.cfg, self.key, self.log)
    self.save_signed_img()
    self.destory_tdir()

class CmdOpts:
  def __init__(self):
    self.parse_cmdopts()
    self.mk_dimg_dir()

  def get_raw_image_path(self, project_name):
    for parent,dirnames,filenames in os.walk("release"): 
      if project_name in parent:
        print(parent)
        return parent

  def parse_cmdopts(self):
    usage = "usage: %prog [options] arg"  
    parser = optparse.OptionParser(usage)  
    parser.add_option("-k", "--keys", dest="keys_dir", help="verified boot keys directory")
    parser.add_option("-c", "--cfg", dest="cfg_dir", help="verified boot config directory")
    parser.add_option("-s", "--src_img", dest="src_img_dir", help="source images directory")
    parser.add_option("-d", "--dst_img", dest="dst_img_dir", help="destination images directory")
    (options, args) = parser.parse_args()
    for opt_item in (options.src_img_dir, options.keys_dir, options.dst_img_dir, options.cfg_dir):
      if not opt_item:
        error_exit("please check out input arguements\n\ttype '{}  -h' for help")
    self.set_simg_dir(os.path.abspath(options.src_img_dir))
    self.keys_dir = os.path.abspath(options.keys_dir)
    self.dimg_dir = os.path.abspath(options.dst_img_dir)
    self.cfg_dir = os.path.abspath(options.cfg_dir)

  def set_simg_dir(self, dir_name):
    self.simg_dir = None
    self.sw_ver = None
    for parent,dirnames,filenames in os.walk(dir_name): 
      if os.path.basename(parent) == "verified_boot_cfgfiles":
        self.simg_dir = os.path.dirname(parent)
        self.sw_ver = os.path.basename(self.simg_dir)
        break
    for parent,dirnames,filenames in os.walk(dir_name): 
      for f in filenames:
        self.simg_dir = parent
        self.sw_ver = os.path.basename(self.simg_dir)
        break

    if (self.simg_dir is None) or (self.sw_ver is None):
      error_exit("no image to sign, eixt")

  def mk_dimg_dir(self):
    if not os.path.exists(self.dimg_dir):
      os.makedirs(self.dimg_dir)
    
def main():  
  opts = CmdOpts()
  logs = Log(opts)
  keys = KeyFile(opts)
  cfgs = CfgFiles(opts)
  img = ImgFile(opts, keys, cfgs, logs)
  img.sign()

if __name__ == '__main__':
  main()
