//专为任意点显示而存在的代码
/////-----       图形显示模式	    -----/////
.define PAINT_COVER		0
.define PAINT_SET		1
.define PAINT_CLR		2
.define PAINT_REV		3
.define PAINT_RECO		4

.ram
.external _CurPaintMode			//Painting mode
.external _Shift_Data_buf//:	.DW 	5	 Dup(0)
.external _Cover_Data_buf
.var Data_Add
.var Num_buf
.var Cover_Mask
.var Bit_Num

.external _Shift_Buff
.code
.public _F_Data_Cover
_F_Data_Cover:
	push r5 to [sp]
	r5 = sp+1
	push r2,r4 to [sp]
	r1 = [r5+3]						//Get the Buffer Address
	r2 = [r5+4]						//Get Shift Num
	r3 = [r5+5]						//Get All the Data`s number want shift
	r4 = [r5+6]						//Get bit Numbers
	call F_Data_Move
	[Num_buf] = r3
	r1 = _Shift_Data_buf
	r1 = r1+3
	[Data_Add] = r1// to [sp]
	[Bit_Num] = r4
	test r2,0xffff
	jnz F_Data_Cover_L1
	jmp F_Data_Cover_NotS
F_Data_Cover_L1:					//Shift the Number of the source begin....
	r1 = [Data_Add]
	r3 = [Num_buf]
	r4 = 0
	r4 = r4 lsl 4
F_Data_Cover_L2:
	r4 = [r1]
	r4 = r4 rol 1
	[r1--] = r4
	r4 = r4 lsl 3
	r3-=1
	jnz	F_Data_Cover_L2
	r2-=1
	jnz	F_Data_Cover_L1				//...end
F_Data_Cover_NotS:
	r3 = [Num_buf]					//Get the data number for all(word)
	r4 = _Shift_Buff			//index to Data of LCD Read Back
	r4 += 3
	r2 = _Shift_Data_buf		//index to Data of Source
	r2 += 3
	r1 = [_CurPaintMode]
	jz ?F_Data_Cover_Cover
	cmp r1,PAINT_RECO//VER
	jnz	?F_Data_Cover_Not
?F_Data_Cover_Cover:
	r5 = _Cover_Data_buf
	r5 += 3
?F_Data_Cover_L21:
	r1 = [r4--]
	r1 = r1 & [r5--]
	r1 = r1 | [r2]
	[r2--] = r1
	r3 -= 1
	jnz ?F_Data_Cover_L21
	jmp F_Data_Cover_Exit
?F_Data_Cover_Not:
	cmp r1,PAINT_SET
	jnz ?F_Data_Cover_Not1
?F_Data_SET_L1:
	r1 = [r4--]
	r1 = r1|[r2]
	[r2--] = r1
	r3-=1
	jnz ?F_Data_SET_L1
	jmp F_Data_Cover_Exit
?F_Data_Cover_Not1:
	cmp r1,PAINT_CLR
	jnz ?F_Data_Cover_Not2
?F_Data_CLR_L1:
	r1 = [r2]
	r1 = r1^0xffff
	r1 = r1&[r4--]
	[r2--] = r1
	r3-=1
	jnz ?F_Data_CLR_L1
	jmp	F_Data_Cover_Exit
?F_Data_Cover_Not2:
?F_Data_REV_L1:
	r1 = [r4--]
	r1 = r1^[r2]
	[r2--] = r1
	r3-=1
	jnz ?F_Data_REV_L1
F_Data_Cover_Exit:
	pop r2,r4 from [sp]
	pop r5 from [sp]
	retf
	
F_Data_Move:
	push r2,r5 to [sp]
	r3 = r4&0x0f
	r4 = r4 lsr 4
	r1 = r1+r4
	test r3,0xff
	jnz F_Data_Move_NotAd
	r1-=1
	jmp	F_Data_Move_22
F_Data_Move_NotAd:
	r4 = r4+1
F_Data_Move_22:
	r2 = _Shift_Data_buf
	r2 = r2+3
	r5 = [_CurPaintMode]
	cmp r5,PAINT_RECO
	jnz F_Data_Move_Loop
F_Data_Move_Loop1:
	r3 = [r1--]
	r3 = r3^0xffff
	[r2--] = r3				//bug1
	r4-=1
	jnz	F_Data_Move_Loop1
	jmp F_Data_Move_Exit
F_Data_Move_Loop:
	r3 = [r1--]
	[r2--] = r3				//bug1
	r4-=1
	jnz	F_Data_Move_Loop
F_Data_Move_Exit:
	pop r2,r5 from [sp]
	retf

//************************************************************************
// 
// 设计者: xinqiang
// 描  述:
// 日  期: 2005.6.20
// 版本号: 1.0
//*************************************************************************
.public _F_Cover_Initial
_F_Cover_Initial:
	push r5 to [sp]
	r5 = sp+1
	push r1,r4 to [sp]
	r2 = [r5+4]				//Get the shift number
//	r4 = [r5+5]
	r1 = [r5+3]				//Get the bit number
	jnz	?F_Cover_NZ
	jmp	?F_Cover_Exit	
?F_Cover_NZ:
	r5 = _Cover_Data_buf
	r5+=3
	r3 = 4
	r4 = 0xffff
	[r5] = r4
	r4 = 0
	r4 = r4 lsl 4
?F_Cover_Loop1:
	r4 = [r5]
	r4 = r4 rol 1
	[r5--] = r4
	r4 = r4 rol 3
	r3-=1
	jnz ?F_Cover_Loop1
	r5+=4
	r4 = 0
	r4 = r4 lsl 4
	r3 = 4
	r1-=1
	jnz ?F_Cover_Loop1
	test r2,0xffff
	jnz	?F_Cover_Loop2
	jmp	?F_Cover_Exit
?F_Cover_Loop2:
	r4 = 0xffff;
	r4 = r4 lsl 4
	r3 = 4
?F_Cover_Loop3:
	r4 = [r5]
	r4 = r4 rol 1
	[r5--] = r4
	r4 = r4 lsl 3
	r3-=1
	jnz ?F_Cover_Loop3
	r5+=4
	r2-=1
	jnz ?F_Cover_Loop2
?F_Cover_Exit:
	pop r1,r4 from [sp]
	pop r5 from [sp]
	retf

//****************************************************************************************************
// 128X64I液晶底层驱动画圆误差计算  W_Red_Err(int *a,int *b,int *r)
// 设计者: powerint
// 描  述: 系统初始化
// 日  期: 2004.5.9
// 版本号: 1.2
//****************************************************************************************************
.PUBLIC _W_Red_Err
_W_Red_Err: push bp,bp to [sp]
            bp = sp+1
            r3 = [bp+3]
            r3 = [r3]
            r3 = r3+1
            r4 = r3
            mr = r3*r4,us
            r1 = r3
            r3 = [bp+4]
            r3 = [r3]
            r4 = r3
            mr = r3*r4,us
            r1+= r3
            r3 = [bp+5]
            r3 = [r3]
            r4 = r3
            mr = r3*r4,us
            r1-=r3
            jl squ1
            r4 = [bp+4]
            r3 = [r4]
            r1-= r3
            jl squ1
            r3-=1
            [r4] = r3;
      squ1: r4 = [bp+3]
            r3 = [r4]
            r3+=1
            [r4] = r3
            pop bp,bp from [sp]
            retf       