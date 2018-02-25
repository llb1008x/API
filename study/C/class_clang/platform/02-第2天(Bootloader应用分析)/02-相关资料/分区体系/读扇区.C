/**********************************************************************************************/
/*检测分区*/
/****************将mbr中的一条分区记录放入分区信息数组ptable[26]****************/
void put_mbr_in_partation(unsigned char chosen_disk,int p, struct PARTATION_TABLE part)
{
	unsigned char str[20];
	int hdd_num;
	hdd_num=chosen_disk-0x80;
	ptable[hdd_num][p].boot_ind = part.boot_ind;
	ptable[hdd_num][p].head = part.head;
	ptable[hdd_num][p].sector = part.sector;
	ptable[hdd_num][p].cyl = part.cyl;
	ptable[hdd_num][p].sys_ind = part.sys_ind;
	ptable[hdd_num][p].end_head = part.end_head;
	ptable[hdd_num][p].end_sector = part.sector;
	ptable[hdd_num][p].end_cyl = part.end_cyl;
	ptable[hdd_num][p].lba = part.lba;
	ptable[hdd_num][p].size = part.size;
	ptable[hdd_num][p].size_MB = ptable[hdd_num][p].size >> 11;
	
	sprintf(str, "Partation: %-2d", (p+1));
 	outtext_en(56,83+cursornum*15,WHITE,str,1);	
	
	cursornum++;
}

int get_ebr(unsigned char chosen_disk,unsigned long long next_ebr_beginsec)
{
	int err = 0;
	unsigned char buffer[512];
	int i;
	int ext_part_tab[2] = { 0x1be, 0x1ce };	/*扩展分区表的开始位置 */
	int temp_begin;
	int temp_end;
	unsigned long long temp_lba;
	unsigned long long temp_size;
	struct PARTATION_TABLE ext_partation_t[2];
	for (i = 0; i < 512; i++) 
		{
		 buffer[i] = 0;
		}
	read_hdd(next_ebr_beginsec,buffer,chosen_disk);
	
	for (i = 0; i < 2; i++) 	/*将4项分区表信息放入结构体数组 */
	{
		temp_begin = 0;
		temp_end = 0;	/*将temp的各bit置0 */
		temp_lba = 0;
		temp_size = 0;
		ext_partation_t[i].boot_ind = buffer[ext_part_tab[i]];
		ext_partation_t[i].head = buffer[ext_part_tab[i] + 1];
		ext_partation_t[i].sector =
		    buffer[ext_part_tab[i] + 2] & 0x3f;
		temp_begin = buffer[ext_part_tab[i] + 2] & 0xc0;
		ext_partation_t[i].cyl = (temp_begin << 2) + buffer[ext_part_tab[i] + 3];	/*计算出10bit的cyl，放到int型中 */
		ext_partation_t[i].sys_ind = buffer[ext_part_tab[i] + 4];
		ext_partation_t[i].end_head = buffer[ext_part_tab[i] + 5];
		ext_partation_t[i].end_sector =
		    buffer[ext_part_tab[i] + 6] & 0xc0;
		temp_end = buffer[ext_part_tab[i] + 6] & 0xc0;
		ext_partation_t[i].cyl =
		    (temp_end << 2) + buffer[ext_part_tab[i] + 7];
		temp_lba = buffer[ext_part_tab[i] + 11];	/*temp_lba为unsigned long */
		temp_lba =
		    (((((temp_lba << 8) +
			buffer[ext_part_tab[i] + 10]) << 8) +
		      buffer[ext_part_tab[i] + 9]) << 8) +
		    buffer[ext_part_tab[i] + 8];
		ext_partation_t[i].lba = temp_lba;
		temp_size = buffer[ext_part_tab[i] + 15];
		temp_size =
		    (((((temp_size << 8) +
			buffer[ext_part_tab[i] + 14]) << 8) +
		      buffer[ext_part_tab[i] + 13]) << 8) +
		    buffer[ext_part_tab[i] + 12];
		ext_partation_t[i].size = temp_size;
	}
	ext_partation_t[0].lba = next_ebr_beginsec + ext_partation_t[0].lba;
	next_ebr_beginsec = ebr_beginsec + ext_partation_t[1].lba;
	
	put_mbr_in_partation(chosen_disk,part_num, ext_partation_t[0]);
	part_num++;
	
	if (ext_partation_t[1].sys_ind != 0)
		if ((err = get_ebr(chosen_disk, next_ebr_beginsec)) == 0) 
			{
			 return 0;
			}
			return 1;

}

/***********************获取MBR信息**********************/
int get_mbr(unsigned char chosen_disk)
{
	int err = 0;
	unsigned long long sec_no = 0;
	unsigned char buffer[512];
	int i;
	int base_part_tab[4] = { 0x1be, 0x1ce, 0x1de, 0x1ee };	/*主分区表的开始位置 */
	int temp_begin;
	int temp_end;
	unsigned long long temp_lba;
	unsigned long long temp_size;
	struct PARTATION_TABLE partation_t[4];
	part_num = 0;
	ebr_beginsec = 0;
	for (i = 0; i < 512; i++) {
		buffer[i] = 0;
	}
	
	read_hdd(0,buffer,chosen_disk);

	for (i = 0; i < 4; i++) 	/*将4项分区表信息放入结构体数组 */
	{	temp_begin = 0;
		temp_end = 0;	/*将temp的各bit置0 */
		temp_lba = 0;
		temp_size = 0;
		partation_t[i].boot_ind = buffer[base_part_tab[i]];
		partation_t[i].head = buffer[base_part_tab[i] + 1];
		partation_t[i].sector =
		    buffer[base_part_tab[i] + 2] & 0x3f;
		temp_begin = buffer[base_part_tab[i] + 2] & 0xc0;
		partation_t[i].cyl = (temp_begin << 2) + buffer[base_part_tab[i] + 3];	/*计算出10bit的cyl，放到int型中 */
		partation_t[i].sys_ind = buffer[base_part_tab[i] + 4];
		partation_t[i].end_head = buffer[base_part_tab[i] + 5];
		partation_t[i].end_sector =
		    buffer[base_part_tab[i] + 6] & 0xc0;
		temp_end = buffer[base_part_tab[i] + 6] & 0xc0;
		partation_t[i].cyl =
		    (temp_end << 2) + buffer[base_part_tab[i] + 7];
		temp_lba = buffer[base_part_tab[i] + 11];	/*temp_lba为unsigned long */
		temp_lba =
		    (((((temp_lba << 8) +
			buffer[base_part_tab[i] + 10]) << 8) +
		      buffer[base_part_tab[i] + 9]) << 8) +
		    buffer[base_part_tab[i] + 8];
		partation_t[i].lba = temp_lba;
		temp_size = buffer[base_part_tab[i] + 15];
		temp_size =
		    (((((temp_size << 8) +
			buffer[base_part_tab[i] + 14]) << 8) +
		      buffer[base_part_tab[i] + 13]) << 8) +
		    buffer[base_part_tab[i] + 12];
		partation_t[i].size = temp_size;


			/**********************************/
			if((partation_t[i].boot_ind==0x80)||(partation_t[i].boot_ind==0x00)||(partation_t[i].size < hdd[chosen_disk-0x80]))
		switch (partation_t[i].sys_ind) 
			{
		case 0x01:
		case 0x04:
		case 0x06:
		case 0x07:
		case 0x0b:
		case 0x0c:
		case 0x0e:
		case 0x81:
		case 0x82:
		case 0x83:
		case 0x85:
		case 0x8e:
		case 0xfd:
				put_mbr_in_partation(chosen_disk,part_num,partation_t[i]);
				part_num++;
				break;			
		case 0x05:
		case 0x0f:
				ebr_beginsec = partation_t[i].lba;				
				if ((err = get_ebr(chosen_disk,ebr_beginsec)) == 0) 
				{}
				break;
			
		default:
				;
			}
	}
	if(part_num == 0)
		return 0;
	return 1;


}



/*检测硬盘*/
void main_frame()
{  unsigned char str[20];
	unsigned char dl,i,f;
        int n=0;
        int k;
	
    for(i=0;i<4;i++)
      {nexthdd[i]=(100+i);
       }
    for(dl=0x80;dl<0x84;dl++)
    {f=Parameter(dl);
     if(f==1)
        {
         break;
        }

      k=dl-0x80;
      hdd[k]=P1(dl);
      nexthdd[k]=cursornum;				/*nexthdd中记录着磁盘显示位置，相邻2个磁盘显示位置之间为分区显示位置*/
      
     sprintf(str, "HDD %-2d:", k);
     outtext_en(10,83+cursornum*15,WHITE,str,1);
     cursornum++;
     get_mbr(dl);				     /*n=detect_partation(dl,n,k);*/
    }
   
}