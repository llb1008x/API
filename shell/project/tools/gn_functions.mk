[A-Z] := A B C D E F G H I J K L M N O P Q R S T U V W X Y Z #
[a-z] := a b c d e f g h i j k l m n o p q r s t u v w x y z #
[0-9] := 0 1 2 3 4 5 6 7 8 9 #
[A-F] := A B C D E F #

tr = $(strip $(eval __gmsl_t := $3) $(foreach c, $(join $(addsuffix :,$1),$2), $(eval __gmsl_t := $(subst $(word 1,$(subst :, ,$c)),$(word 2,$(subst :, ,$c)), $(__gmsl_t))))$(__gmsl_t))

uc = $(call tr,$([a-z]),$([A-Z]),$1)
lc = $(call tr,$([A-Z]),$([a-z]),$1)
gn_warning = $(if $(filter 1,$(strip $(GN_DEBUG))),$(warning $1))
gn_error = $(if $(filter 1,$(strip $(GN_DEBUG))),$(error $1))

tmp_touch = $(shell echo "" > $(GN_KERNEL_TMP_FILE))

define .if-cfg-on
    $(if $(filter-out $(strip $(GN_FILTER_NO_LST)),$(strip $($(1)))),$(2),$(3))
endef

define .tmp_add
    $(shell echo $(1) >> $(GN_KERNEL_TMP_FILE))
endef

define gionee.custom.generate-macros
    $(strip $(foreach t,$(AUTO_ADD_GLOBAL_DEFINE_BY_NAME),$(call .if-cfg-on,$(t),-D$(call uc,$(t))))
        $(foreach t,$(AUTO_ADD_GLOBAL_DEFINE_BY_NAME_VALUE),$(call .if-cfg-on,$(t),-D$(call uc,$(t))=$($(t)))))
endef

define gionee.custom.generate-tmp-cfg
    aa := $(call tmp_touch)
    $(strip $(foreach t,$(AUTO_ADD_GLOBAL_DEFINE_BY_NAME),$(if $(filter-out $(strip $(GN_FILTER_NO_LST)),$(strip $($(t)))),$(call .tmp_add,$(t)=y),$(call .tmp_add,$(t)=n)))
        $(foreach t,$(AUTO_ADD_GLOBAL_DEFINE_BY_NAME_VALUE),$(if $(filter-out $(strip $(GN_FILTER_NO_LST)),$(strip $($(t)))),$(call .tmp_add,$(t)=$($(t))),$(call .tmp_add,$(t)=n)))
        )
endef

