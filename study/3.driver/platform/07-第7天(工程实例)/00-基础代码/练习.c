

#include <linux/gpio_keys.h>



/* Configure GPIOs for gpio Keys */
static struct gpio_keys_button am335x_evm_gpio_gpio_buttons[] = {

	{
		.code                   = KEY_UP,
		.gpio                   = GPIO_TO_PIN(1, 20),
		.active_low             = true,
		.desc                   = "key-up",
		.type                   = EV_KEY,
		.wakeup                 = 1,
	},
	{
		.code                   = KEY_DOWN,
		.gpio                   = GPIO_TO_PIN(1, 21),
		.active_low             = true,
		.desc                   = "key-down",
		.type                   = EV_KEY,
		.wakeup                 = 1,
	},
	{
		.code                   = KEY_LEFT,
		.gpio                   = GPIO_TO_PIN(1, 22),
		.active_low             = true,
		.desc                   = "key-left",
		.type                   = EV_KEY,
		.wakeup                 = 1,
	},
	{
		.code                   = KEY_RIGHT,
		.gpio                   = GPIO_TO_PIN(1, 23),
		.active_low             = true,
		.desc                   = "key-right",
		.type                   = EV_KEY,
		.wakeup                 = 1,
	},
	{
		.code                   = KEY_ENTER,
		.gpio                   = GPIO_TO_PIN(1, 24),
		.active_low             = true,
		.desc                   = "key-center",
		.type                   = EV_KEY,
		.wakeup                 = 1,
	},
	{
		.code					= KEY_Q,
		.gpio					= GPIO_TO_PIN(2,0),
		.active_low 			= true,
		.desc				= "key-q",
		.type					= EV_KEY,
		.wakeup 				= 1,
	},
};

static struct gpio_keys_platform_data am335x_evm_gpio_gpio_key_info = {
	.buttons        = am335x_evm_gpio_gpio_buttons,
	.nbuttons       = ARRAY_SIZE(am335x_evm_gpio_gpio_buttons),
};

static struct platform_device am335x_evm_gpio_keys = {
	.name   = "gpio-keys",
	.id     = -1,
	.dev    = {
		.platform_data  = &am335x_evm_gpio_gpio_key_info,
	},
};
