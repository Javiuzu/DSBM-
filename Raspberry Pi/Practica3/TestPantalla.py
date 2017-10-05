import DriverPantalla as dp



dp.Config_Pins()
dp.SPI_TFT_Reset()

for i in range(240):
	for j in range(320):
		dp.SPI_TFT_pixel(i,j,0)
dp.Free_Pins()

