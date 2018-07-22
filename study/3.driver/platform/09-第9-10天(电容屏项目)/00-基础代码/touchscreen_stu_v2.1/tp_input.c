	goodix->input_dev->evbit[0] = BIT_MASK(EV_SYN) | BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS) ;
	goodix->input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	goodix->input_dev->absbit[0] = BIT(ABS_X) | BIT(ABS_Y); // for android
	set_bit(ABS_PRESSURE, goodix->input_dev->absbit);
	
	input_set_abs_params(goodix->input_dev, ABS_X, 0, SCREEN_MAX_HEIGHT, 0, 0);
	input_set_abs_params(goodix->input_dev, ABS_Y, 0, SCREEN_MAX_WIDTH, 0, 0);
	input_set_abs_params(goodix->input_dev, ABS_PRESSURE, 0 , 255, 0, 0);
	