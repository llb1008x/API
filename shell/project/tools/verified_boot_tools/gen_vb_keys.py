#!/usr/bin/env python3

import os, sys, subprocess, optparse

debug = True
#debug = False

def shell_cmd(cmd):
  if debug == True:
    print("shell cmd:\n{}\n".format(cmd))
  return subprocess.call(cmd, shell=True)

class Keys:
  def __init__(self, keys_output):
    self.curdir = os.path.abspath(os.path.curdir)
    self.generated_keys_dir = keys_output
    self.prik_pem = os.path.join(self.generated_keys_dir, "prik/phone/pem")
    self.prik4pc = os.path.join(self.generated_keys_dir, "prik/pc")
    self.prik_text = os.path.join(self.generated_keys_dir, "prik/phone/text")
    self.pubk_pem = os.path.join(self.generated_keys_dir, "pubk/phone/pem")
    self.pubk_text = os.path.join(self.generated_keys_dir, "pubk/phone/text")
    self.pubk4pc = os.path.join(self.generated_keys_dir, "pubk/pc")
    self.cert_key_list = ("arm7", "boot", "lk", "md1arm7", "md1", "md3", "recovery", "atf", "dsp", "logo", "md1dsp", "md1rom", "md3rom", "tee")
    self.key1024_name_list = ("dapl", )
    self.rsa_text_keys = ("sbc", "daa", "dapl2048")
    self.set_dirs = self.set_path()
 
  def set_path(self):
    for d in (self.prik_pem, self.prik4pc, self.prik_text, self.pubk_pem, self.pubk_text, self.pubk4pc):
      if not os.path.exists(d):
        subprocess.call("mkdir -p {}".format(d), shell=True)

  def get_rsa_prik_pem(self, file_o, rsa_len):
    shell_cmd("openssl genrsa -out {} {}".format(file_o, rsa_len))

  def get_rsa_pubk_pem(self, file_i, file_o):
    shell_cmd("openssl rsa -in {} -pubout > {}".format(file_i, file_o))

  def get_rsa_text(self, file_i, file_o):
    shell_cmd("openssl rsa -text -in {} -pubout > {}".format(file_i, file_o))

  def get_digital_raw(self, file_i, start, end):
    fi = open(file_i, "r")
    outstring = ""
    for line in fi:
      str_l = line.strip()
      if str_l.startswith(start):
        outstring = ""
      elif str_l.startswith(end):
        break
      else:
        outstring += str_l
    fi.close()
    return outstring

  def save2file(self, file_o, content):
    fo = open(file_o, "w")
    fo.write(content)
    fo.close()

  def remove_str_first_zeros(self, strings):
    s_list = strings.split(":")
    while True:
      if s_list[0] == "00":
        s_list.pop(0)
      else:
        break
    return s_list

  def list2str_continuity(self, l):
    output = ""
    length = len(l)
    for i in range(length):
      output += l[i]
    return output

  def list2str_interval1(self, l):
    output = ""
    length = len(l)
    for i in range(length):
      if (i == (length - 1)):
        output = output + "0x" + l[i]
      else:
        output = output + "0x" + l[i] + ", "
    return output

  def list2str_interval2(self, l):
    output = ""
    length = len(l)
    for i in range(length):
      if ((i % 2) == 0):
        output = output + "0x" + l[i]
      elif (i == (length - 1)):
        output = output + l[i]
      else:
        output = output + l[i] + ", "
    return output

  def list2str_multiline(self, l):
    output = ""
    length = len(l)
    for i in range(length):
      if (i == (length - 1)):
        output = output + "0x" + l[i]
      elif (((i + 1) % 16) == 0):
        output = output + "0x" + l[i] + ", \\\n                   "
      else:
        output = output + "0x" + l[i] + ", "
    return output

  def generate_pem_key(self, key_name, rsa_len):
    if key_name in self.cert_key_list:
      prik = os.path.join(self.prik_pem, key_name + "_privk2.pem")
    else:
      prik = os.path.join(self.prik_pem, key_name + "_privk.pem")
    pubk = os.path.join(self.pubk_pem, key_name + "_pubk.pem")
    self.get_rsa_prik_pem(prik, rsa_len)
    self.get_rsa_pubk_pem(prik, pubk)
    shell_cmd("cp {} {}".format(pubk, self.prik_pem))

  def generate_text_key(self, key_name):
    pem_prik = os.path.join(self.prik_pem, key_name + "_privk.pem")
    if not os.path.exists(pem_prik):
      return
    text_key = os.path.join(self.prik_text, key_name)
    text_pub_key = os.path.join(self.pubk_text, key_name + "_pubk.ini")
    text_pri_key = os.path.join(self.prik_text, key_name + "_prik.ini")
    self.get_rsa_text(pem_prik, text_key)
    pubk_raw_str = self.get_digital_raw(text_key, "modulus:", "publicExponent:")
    prik_raw_str = self.get_digital_raw(text_key, "privateExponent:", "prime1:")
    pubk_list = self.remove_str_first_zeros(pubk_raw_str)
    prik_list = self.remove_str_first_zeros(prik_raw_str)
    pubk_oneline_continuity = self.list2str_continuity(pubk_list)
    prik_oneline_continuity = self.list2str_continuity(prik_list)
    pubk_oneline_interval1 = self.list2str_interval1(pubk_list)
    prik_oneline_interval1 = self.list2str_interval1(prik_list)
    pubk_oneline_interval2 = self.list2str_interval2(pubk_list)
    prik_oneline_interval2 = self.list2str_interval2(prik_list)
    pubk_multiline = self.list2str_multiline(pubk_list)
    prik_multiline = self.list2str_multiline(prik_list)
    file_content = pubk_raw_str + "\n\n" + pubk_oneline_continuity + "\n\n" + pubk_oneline_interval1 + "\n\n" + pubk_oneline_interval2 + "\n\n" + pubk_multiline + "\n\n"
    self.save2file(text_pub_key, file_content)
    file_content = prik_raw_str + "\n\n" + prik_oneline_continuity + "\n\n" + prik_oneline_interval1 + "\n\n" + prik_oneline_interval2 + "\n\n" + prik_multiline + "\n\n"
    self.save2file(text_pri_key, file_content)

    if key_name == "sbc":
      fo_name = os.path.join(self.prik4pc, "SBC_Key.ini")
      file_content = "[SBC public key] \npublic_key_e = \"00010001\"\npublic_key_n = \"" + pubk_oneline_continuity
      file_content = file_content + "\"\npublic_key_n_hex = \"" + pubk_oneline_interval1
      file_content = file_content + "\"\n[SBC private key]\nprivate_key_e = \"00010001\"\nprivate_key_d = \"" 
      file_content = file_content + prik_oneline_continuity + "\"\nprivate_key_n = \"" + pubk_oneline_continuity + "\"\n"
      self.save2file(fo_name, file_content)

      fo_name = os.path.join(self.pubk4pc, "SBC_Key.ini")
      file_content = "[SBC public key] \npublic_key_e = \"00010001\"\npublic_key_n = \"" + pubk_oneline_continuity + "\"\n"
      file_content = file_content + "public_key_n_hex = \"" + pubk_oneline_interval1 + "\""
      self.save2file(fo_name, file_content)

      fo_name = os.path.join(self.prik_pem, "sbc_privk.ini")
      file_content = "[Code Sig Key]\nprivate_key_d = \"" + prik_oneline_continuity + "\"\n" + "private_key_n = \"" 
      file_content = file_content + pubk_oneline_continuity + "\"\n" + "public_key_e = \"010001\"\npublic_key_n = \""
      file_content = file_content + pubk_oneline_continuity + "\"\n"
      file_content = file_content + "\n[Install Sig Key]\npublic_key_e = \"010001\"\npublic_key_n = \"" + pubk_oneline_continuity
      file_content = file_content + "\"\n\n[Anticlone key]\nanticlone_key = \"0x11111111, 0x22222222, 0x33333333, 0x44444444\"\n\n"
      self.save2file(fo_name, file_content)
    elif key_name == "daa":
      fo_name = os.path.join(self.prik4pc, "DAA_Key.ini")
      file_content = "[DAA private key]\nprivate_key_e = \"0x0001, 0x0001\"\nprivate_key_n = \"" + pubk_oneline_interval2
      file_content = file_content + "\"\nprivate_key_d = \"" + prik_oneline_interval2 
      file_content = file_content + "\"\n\n[DAA public key]\npublic_key_e = \"00010001\"\npublic_key_n = \"" + pubk_oneline_continuity + "\"\n\n"
      self.save2file(fo_name, file_content)

      fo_name = os.path.join(self.pubk4pc, "DAA_Key.ini")
      file_content = "[DAA public key]\npublic_key_e = \"00010001\"\npublic_key_n = \"" + pubk_oneline_continuity + "\"\n\n"
      self.save2file(fo_name, file_content)
    elif key_name == "dapl2048":
      fo_name = os.path.join(self.prik4pc, "DAPL2048_Key.ini")
      file_content = "[DAPL2048 private key]\n# Authentication Parameter (N, D) for RSA2048 (E=65537, 0x10001)\nAUTH_PARAM_N = "
      file_content = file_content + pubk_oneline_continuity + "\nAUTH_PARAM_D = " + prik_oneline_continuity
      self.save2file(fo_name, file_content)

      fo_name = os.path.join(self.pubk4pc, "DAPL2048_Key.ini")
      file_content = "[DAPL2048 public key]\nAUTH_PARAM_N = " + pubk_oneline_continuity
      self.save2file(fo_name, file_content)
    elif key_name == "dapl":
      fo_name = os.path.join(self.prik4pc, "DAPL_Key.ini")
      file_content = "[DAPL private key]\nprivate_key_e = 10001\nprivate_key_n = "
      file_content = file_content + pubk_oneline_continuity + "\nprivate_key_d = " + prik_oneline_continuity
      self.save2file(fo_name, file_content)

      fo_name = os.path.join(self.pubk4pc, "DAPL_Key.ini")
      file_content = "[DAPL private key]\nprivate_key_e = 10001\nprivate_key_n = " + pubk_oneline_continuity
      self.save2file(fo_name, file_content)

  def gn_raw_key(self):
    for kn in self.cert_key_list:
      self.generate_pem_key(kn, "2048")
    for kn in self.rsa_text_keys:
      self.generate_pem_key(kn, "2048")
    for kn in self.key1024_name_list:
      self.generate_pem_key(kn, "1024")

  def format_key(self):
    for kn in self.rsa_text_keys:
      self.generate_text_key(kn)
    for kn in self.key1024_name_list:
      self.generate_text_key(kn)

  def generate_gionee_keys(self):
    self.gn_raw_key()
    self.format_key()

class CmdOpts:
  def __init__(self):
    self.parse_cmdopts()

  def get_raw_image_path(self, project_name):
    for parent,dirnames,filenames in os.walk("release"): 
      if project_name in parent:
        print(parent)
        return parent

  def parse_cmdopts(self):
    usage = "usage: %prog [options] arg"  
    parser = optparse.OptionParser(usage)  
    parser.add_option("-o", "--output", dest="keys_dir", help="keys output directory")
    (options, args) = parser.parse_args()
    if options.keys_dir is None:
      self.keys_dir = os.path.join(os.path.abspath(os.path.curdir), "gionee_verified_boot_keys")
    else:
      self.keys_dir = os.path.join(os.path.abspath(options.keys_dir), "gionee_verified_boot_keys")
    
def main():  
  opts = CmdOpts()
  keys = Keys(opts.keys_dir)
  keys.generate_gionee_keys()
  sys.exit(0)

if __name__ == '__main__':
  main()
