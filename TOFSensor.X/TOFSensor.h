/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */
/**
 * @Function Sensor_Set_SendDir()
 * @param SendDir_flag
 * @return None
 * @brief  Sets flag to either active send or passive read
 * @author  */
void Sensor_Set_SendDir(int SendDir_flag);

/**
 * @Function Sensor_Set_SendDir()
 * @param SendDir_flag
 * @return Filtered distance value obtained from sensor
 * @brief Gets distance value from sensor reading
 * @author  */
unsigned short int Sensor_Get_AveRange(void);