#include "multidriver.h"

InterfacePosition2d::InterfacePosition2d(player_devaddr_t addr,
		multidriver* driver, ConfigFile* cf, int section) :
		Interface(addr, driver, cf, section) {
	this->conf.frontL = cf->ReadInt(section, "frontL", 1);
	this->conf.frontR = cf->ReadInt(section, "frontR", 2);
	this->conf.rearL = cf->ReadInt(section, "rearL", 3);
	this->conf.rearR = cf->ReadInt(section, "rearR", 4);
	this->conf.servoFR = cf->ReadInt(section, "servofr", 5);
	this->conf.servoFL = cf->ReadInt(section, "servofl", 6);
	this->conf.servoRR = cf->ReadInt(section, "servorr", 7);
	this->conf.servoRL = cf->ReadInt(section, "servorl", 8);
	this->conf.rationSpeedToServo = 400;
	this->conf.maxSpeed = 1023;
	this->conf.aToAngle = 57.4;
	std::string pos2dType = "POS2D4TIRES";
	pos2dType = cf->ReadString(section, "pos2dtype", pos2dType.data());
	if (pos2dType == "PPOS2D4TIRES")
		this->conf.type = POS2D4TIRES;
	else if (pos2dType == "POS2D8TIRES")
		this->conf.type = POS2D8TIRES;
	else if (pos2dType == "POS2D8TOP")
		this->conf.type = POS2D8TOP;
	else if (pos2dType == "POS2D8BUTTON")
		this->conf.type = POS2D8BUTTON;
	else if (pos2dType == "POS2DOMNIDIR")
		this->conf.type = POS2DOMNIDIR;
	this->Interface::PublishSign = cf->ReadInt(section, "publish", 1);
}

int InterfacePosition2d::ProcessMessage(QueuePointer &resp_queue,
		player_msghdr_t* hdr, void* data) {
	if (this->conf.type == POS2D4TIRES) {
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL, this->Interface::addr)) {
			player_position2d_cmd_vel_t* pcmd =
					(player_position2d_cmd_vel_t*) data;

			//mod->SetSpeed(pcmd->vel.px, pcmd->vel.py, pcmd->vel.pa);
			double speed = pcmd->vel.px;
			double turn = pcmd->vel.pa;
			double trsts;
			if ((turn > 0.7 && turn < 0.8) || (turn > -0.8 && turn < -0.7)) {
				trsts = this->conf.rationSpeedToServo;
				this->conf.rationSpeedToServo = 560;
			} else if ((turn > 1.5 && turn < 1.6)
					|| (turn > -1.6 && turn < -1.5)) {
				trsts = this->conf.rationSpeedToServo;
				this->conf.rationSpeedToServo = 258;
			}
			double frontLSpeed = -speed + turn;
			double rearLSpeed = -speed + turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					(int) (frontLSpeed * this->conf.rationSpeedToServo));
			//printf("%d\n",(int) (frontLSpeed * this->conf.rationSpeedToServo));
			MFSetServoRotaSpd(this->conf.rearL,
					(int) (rearLSpeed * this->conf.rationSpeedToServo));
			//printf("%d\n",(int) (rearLSpeed * this->conf.rationSpeedToServo));
			MFSetServoRotaSpd(this->conf.frontR,
					(int) (frontRSpeed * this->conf.rationSpeedToServo));
			//printf("%d\n",(int) (frontRSpeed * this->conf.rationSpeedToServo));
			MFSetServoRotaSpd(this->conf.rearR,
					(int) (rearRSpeed * this->conf.rationSpeedToServo));
			//printf("%d\n",(int) (rearRSpeed * this->conf.rationSpeedToServo));
			MFServoAction();

			this->conf.rationSpeedToServo = trsts;

			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_CAR, this->Interface::addr)) {
			player_position2d_cmd_car_t* pcmd =
					(player_position2d_cmd_car_t*) data;
			//mod->SetSpeed(pcmd->velocity, 0, pcmd->angle);
			double speed = pcmd->velocity;
			double turn = pcmd->angle;
			double frontLSpeed = speed - turn;
			double rearLSpeed = speed - turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					((int) frontLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearL,
					((int) rearLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.frontR,
					((int) frontRSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearR,
					((int) rearRSpeed) * this->conf.rationSpeedToServo);
			MFServoAction();
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL_HEAD, this->Interface::addr)) {
			return -1;
		}
	} else if (this->conf.type == POS2D8TIRES) {
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_POS, this->Interface::addr)) {
			player_position2d_cmd_pos_t* pcmd =
					(player_position2d_cmd_pos_t*) data;
			//mod->GoTo(pcmd->pos.px, pcmd->pos.py, pcmd->pos.pa);
			double turn = pcmd->pos.pa;
			turn *= this->conf.aToAngle;
			if (turn > 300 || turn < 0)
				return -1;
			int pos = (turn * 1023) / 300;
			MFSetServoPos(this->conf.servoFR, pos, 400);
			MFSetServoPos(this->conf.servoFL, pos, 400);
			MFSetServoPos(this->conf.servoRR, pos, 400);
			MFSetServoPos(this->conf.servoRL, pos, 400);
			MFServoAction();
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
				PLAYER_POSITION2D_DATA_STATE, this->Interface::addr)) {
			//fellow the Pos2d8Top
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL, this->Interface::addr)) {
			player_position2d_cmd_vel_t* pcmd =
					(player_position2d_cmd_vel_t*) data;

			//mod->SetSpeed(pcmd->vel.px, pcmd->vel.py, pcmd->vel.pa);
			double speed = pcmd->vel.px;
			double turn = pcmd->vel.pa;
			double frontLSpeed = speed - turn;
			double rearLSpeed = speed - turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					((int) frontLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearL,
					((int) rearLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.frontR,
					((int) frontRSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearR,
					((int) rearRSpeed) * this->conf.rationSpeedToServo);
			MFServoAction();

			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_CAR, this->Interface::addr)) {
			player_position2d_cmd_car_t* pcmd =
					(player_position2d_cmd_car_t*) data;
			//mod->SetSpeed(pcmd->velocity, 0, pcmd->angle);
			double speed = pcmd->velocity;
			double turn = pcmd->angle;
			double frontLSpeed = speed - turn;
			double rearLSpeed = speed - turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					((int) frontLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearL,
					((int) rearLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.frontR,
					((int) frontRSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearR,
					((int) rearRSpeed) * this->conf.rationSpeedToServo);
			MFServoAction();
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL_HEAD, this->Interface::addr)) {
			return -1;
		}
	} else if (this->conf.type == POS2D8TOP) {
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL, this->Interface::addr)) {
			player_position2d_cmd_vel_t* pcmd =
					(player_position2d_cmd_vel_t*) data;

			//mod->SetSpeed(pcmd->vel.px, pcmd->vel.py, pcmd->vel.pa);
			double speed = pcmd->vel.px;
			double turn = pcmd->vel.pa;
			double frontLSpeed = speed - turn;
			double rearLSpeed = speed - turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					((int) frontLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearL,
					((int) rearLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.frontR,
					((int) frontRSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearR,
					((int) rearRSpeed) * this->conf.rationSpeedToServo);
			MFServoAction();

			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_CAR, this->Interface::addr)) {
			player_position2d_cmd_car_t* pcmd =
					(player_position2d_cmd_car_t*) data;
			//mod->SetSpeed(pcmd->velocity, 0, pcmd->angle);
			double speed = pcmd->velocity;
			double turn = pcmd->angle;
			double frontLSpeed = speed - turn;
			double rearLSpeed = speed - turn;
			double frontRSpeed = speed + turn;
			double rearRSpeed = speed + turn;
			if ((int) frontLSpeed > this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = this->conf.maxSpeed;
			if ((int) frontLSpeed < -this->conf.maxSpeed)
				frontLSpeed = rearLSpeed = -this->conf.maxSpeed;
			if ((int) frontRSpeed > this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = this->conf.maxSpeed;
			if ((int) frontRSpeed < -this->conf.maxSpeed)
				frontRSpeed = rearRSpeed = -this->conf.maxSpeed;

			MFSetServoRotaSpd(this->conf.frontL,
					((int) frontLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearL,
					((int) rearLSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.frontR,
					((int) frontRSpeed) * this->conf.rationSpeedToServo);
			MFSetServoRotaSpd(this->conf.rearR,
					((int) rearRSpeed) * this->conf.rationSpeedToServo);
			MFServoAction();
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL_HEAD, this->Interface::addr)) {
			return -1;
		}
	} else if (this->conf.type == POS2D8BUTTON) {
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_POS, this->Interface::addr)) {
			player_position2d_cmd_pos_t* pcmd =
					(player_position2d_cmd_pos_t*) data;
			//mod->GoTo(pcmd->pos.px, pcmd->pos.py, pcmd->pos.pa);
			double turn = pcmd->pos.pa;
			turn *= this->conf.aToAngle;
			if (turn > 300 || turn < 0)
				return -1;
			int pos = (turn * 1023) / 300;
			MFSetServoPos(this->conf.servoFR, pos, 400);
			MFSetServoPos(this->conf.servoFL, pos, 400);
			MFSetServoPos(this->conf.servoRR, pos, 400);
			MFSetServoPos(this->conf.servoRL, pos, 400);
			MFServoAction();
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_DATA,
				PLAYER_POSITION2D_DATA_STATE, this->Interface::addr)) {
			//fellow the Pos2d8Top
			return 0;
		} else if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL_HEAD, this->Interface::addr)) {
			return -1;
		}
	} else if (this->conf.type == POS2DOMNIDIR) {
		return -1;
	}
	return -1;
}

void InterfacePosition2d::Publish(void) {
	return;
}
