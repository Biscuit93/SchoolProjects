package simulation;

public class Entity
{
	public int xSize = 0,  ySize = 0, 
	           xPos = 0,   yPos = 0, 
	           xSpeed = 0, ySpeed = 0,
	           xAccel = 0, yAccel = 0;

	public Entity ( )
	{

	}

	public void update ( )
	{
		xSpeed += xAccel;
		ySpeed += yAccel;

		xPos += xSpeed;
		yPos += ySpeed;
	}
}