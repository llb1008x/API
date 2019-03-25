###########################################################################################################################
#To fix buildnumber for GMS authentication begin
# chenle 2015-09-15 add for overseas project
#**********use model/regions or others to distinguish, if you don't define at here, the default value is $(shell date +%s)
# If you do not deinfe, the default value: BUILD_NUMBER := $(shell date +%s)
# If BUILD_NUMBER := null or space, finally BUILD_NUMBER is null or space in fingerprint.
# Note: only for fixing the fingerprint to add BUILD_NUMBER in this file.
### 1. Google approval, PASS_GMS_CERTIFICATION = yes
### 2. Except for using MP sw to final GMS testing, eg. BA,VF project. don't add anything at here.
### 3. The value of BUILD_NUMBER must not be null or space, You don't have to define BUILD_NUMBER, so it will be a default. 
###########################################################################################################################
ifeq ($(strip $(PASS_GMS_CERTIFICATION)),yes)

    ifneq ($(strip $(GN_OVERSEA_PRODUCT)),yes)
        BUILD_NUMBER := $(GN_GMS_BUILD_NUMBER)
    endif
	ifeq ($(strip $(GN_OVERSEA_ODM)),no)
	###     For Brand GiONEE project begin ###
		ifeq ($(strip $(GN_RO_PRODUCT_MODEL)),S6) 	
		#7521 project,
			BUILD_NUMBER := 1451044232
		#7511 project
		else ifeq ($(strip $(GN_PRODUCT_ORIGIN_MODEL)),WBL7511A)
			#7511GI 16+3 Region: India
			ifneq (,$(filter INDIA_GIONEE NEPAL_GIONEE,$(GN_OVERSEA_CUSTOM)))
				BUILD_NUMBER := 1442251909
			#7511 16+2 GS
			else
				BUILD_NUMBER := 1440735317
			endif
		endif
	###     For Brand GiONEE project end ###
	else
	###     For Brand ODM project begin  ###

	###     For Brand ODM project end  ###	
	endif
endif
###########################################################################################################################
#To fix  buildnumber for GMS authentication end
###########################################################################################################################
