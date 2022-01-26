unsigned in sensorsRead(int i0, int i1, int i2, boolean white_line)
{
	int sum = (Sens[i2] / Kmax) * 10 + (Sens[i1] / Kmax) * 20 + (Sens[i0] / Kmax) * 40;
}
