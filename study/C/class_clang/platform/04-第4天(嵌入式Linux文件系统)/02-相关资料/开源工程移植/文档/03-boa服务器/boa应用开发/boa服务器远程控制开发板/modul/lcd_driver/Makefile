OUTPUT = led_driver.o
OUTPUT_DIR = output

KERNEL = /home/lijun/kernel
CROSSPREFIX = arm-linux-
CFLAGS = -Wall -I$(KERNEL)/include -c -DDEBUG
#CFLAGS += -DEXPORT_SYMTAB -DMODVERSIONS -include $(KERNEL)/include/linux/modversions.h 

DEST = $(foreach fn, $(OUTPUT), $(OUTPUT_DIR)/$(fn))

all: $(OUTPUT_DIR)/install.sh

$(OUTPUT_DIR)/install.sh: $(OUTPUT_DIR) $(DEST)
	@rm $@ -f
	@echo -e " $(foreach fn, $(OUTPUT), "insmod $(fn)\\n")" >> $@
	@echo "Finished!"

$(OUTPUT_DIR):
	@mkdir $@
	@chmod 777 -R $@

$(OUTPUT_DIR)/%.o: %.c
	@echo -n "Compling $^..."
	@$(CROSSPREFIX)gcc $(CFLAGS) $^ -o $@
	@echo "OK"

clean:
	@find . \( -name '*.[oas]' -o -name install.sh \) -type f -print | xargs rm -f
	@echo "Cleaned!"

