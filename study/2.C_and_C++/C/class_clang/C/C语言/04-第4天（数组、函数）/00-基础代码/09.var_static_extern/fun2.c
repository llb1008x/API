static int va = 18;
static int getG(void)
{
	return va;
}
int getO(void)
{
	return getG();
}