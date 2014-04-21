#include <math.h>
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
	MFSetServoMode(this->conf.frontL, 1);
	MFSetServoMode(this->conf.frontR, 1);
	MFSetServoMode(this->conf.rearL, 1);
	MFSetServoMode(this->conf.rearR, 1);
	MFSetServoMode(this->conf.servoFR, 0);
	MFSetServoMode(this->conf.servoFL, 0);
	MFSetServoMode(this->conf.servoRR, 0);
	MFSetServoMode(this->conf.servoRL, 0);
	this->conf.rationSpeedToServo = 1000;
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
	this->conf.id[0] = this->conf.frontL;
	this->conf.id[1] = this->conf.frontR;
	this->conf.id[2] = this->conf.rearL;
	this->conf.id[3] = this->conf.rearR;
	this->conf.id[4] = this->conf.servoFR;
	this->conf.id[5] = this->conf.servoFL;
	this->conf.id[6] = this->conf.servoRR;
	this->conf.id[7] = this->conf.servoRL;
	this->conf.speedAngle[0] = 0;
	this->conf.speedAngle[1] = 0;
	this->conf.speedAngle[2] = 0;
	this->conf.speedAngle[3] = 0;
	this->conf.speedAngle[4] = 0;
	this->conf.speedAngle[5] = 0;
	this->conf.speedAngle[6] = 0;
	this->conf.speedAngle[7] = 0;
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
			if (turn < 0.001 && turn > -0.001)
				this->conf.rationSpeedToServo = 2000;
			else if (speed < 0.001 && speed > -0.001)
				this->conf.rationSpeedToServo = 400;
			if ((turn > 0.7 && turn < 0.8) || (turn > -0.8 && turn < -0.7)) {
				trsts = this->conf.rationSpeedToServo;
				this->conf.rationSpeedToServo = 560;
			} else if ((turn > 1.5 && turn < 1.6)
					|| (turn > -1.6 && turn < -1.5)) {
				trsts = this->conf.rationSpeedToServo;
				this->conf.rationSpeedToServo = 258;
			} else
				this->conf.rationSpeedToServo = 1000;
			double frontLSpeed = speed + turn;
			double rearLSpeed = speed + turn;
			double frontRSpeed = -speed + turn;
			double rearRSpeed = -speed + turn;
			double rsfl, rsrl;
			double rsfr, rsrr;
#ifndef PXA270
			printf(" %f %f\n", frontLSpeed, frontRSpeed);
#endif
			rsfl = rsrl = frontLSpeed * this->conf.rationSpeedToServo;
			rsfr = rsrr = frontRSpeed * this->conf.rationSpeedToServo;
#ifndef PXA270
			printf(" %f %f\n", rsfl, rsfr);
#endif
			if ((int) (frontLSpeed * this->conf.rationSpeedToServo)
					> this->conf.maxSpeed)
				rsfl = rsrl = this->conf.maxSpeed;
			if ((int) (frontLSpeed * this->conf.rationSpeedToServo)
					< -this->conf.maxSpeed)
				rsfl = rsrl = -this->conf.maxSpeed;
			if ((int) (frontRSpeed * this->conf.rationSpeedToServo)
					> this->conf.maxSpeed)
				rsfr = rsrr = this->conf.maxSpeed;
			if ((int) (frontRSpeed * this->conf.rationSpeedToServo)
					< -this->conf.maxSpeed)
				rsfr = rsrr = -this->conf.maxSpeed;

			for (int i = 0; i < 8; i++) {
				if (this->conf.frontL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.frontL, (int) rsfl);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}
#ifndef PXA270
			printf("%d\n", (int) rsfl);
#endif
			for (int i = 0; i < 8; i++) {
				if (this->conf.rearL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.rearL, (int) rsrl);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}
#ifndef PXA270
			printf("%d\n", (int) rsrl);
#endif
			for (int i = 0; i < 8; i++) {
				if (this->conf.frontR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.frontR, (int) rsrr);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}
#ifndef PXA270
			printf("%d\n", (int) rsfr);
#endif
			for (int i = 0; i < 8; i++) {
				if (this->conf.rearR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.rearR, (int) rsrr);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}

#ifndef PXA270
			printf("%d\n", (int) rsrr);
#endif
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
#ifndef PXA270
			printf("turn : %f\n", turn);
#endif
			turn *= this->conf.aToAngle;
#ifndef PXA270
			printf("turn : %f\n", turn);
#endif
			if (turn > 300)
				turn = 300;
			if (turn < -300)
				turn = -300;
			int pos = (turn * 1023) / 300;
#ifndef PXA270
			printf("pos : %d\n", pos);
#endif

			for (int i = 0; i < 8; i++) {
				if (this->conf.servoFR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoFR, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}

			for (int i = 0; i < 8; i++) {
				if (this->conf.servoFL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoFL, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}
			for (int i = 0; i < 8; i++) {
				if (this->conf.servoRR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoRR, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}
			for (int i = 0; i < 8; i++) {
				if (this->conf.servoRL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoRL, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}
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
			double trsts;
			if (turn < 0.001 && turn > -0.001)
				this->conf.rationSpeedToServo = 2000;
			else if (speed < 0.001 && speed > -0.001)
				this->conf.rationSpeedToServo = 400;
			if ((turn > 0.7 && turn < 0.8) || (turn > -0.8 && turn < -0.7)) {
				trsts = this->conf.rationSpeedToServo;
				this->conf.rationSpeedToServo = 560;
			} else if ((turn > 1.5 && turn < 1.6)
					|| (turn > -1.6 && turn < -1.5)) {
				trsts = this->conf.rationSpeedToServo;
				this->conf.rationSpeedToServo = 258;
			} else
				this->conf.rationSpeedToServo = 1000;
			double frontLSpeed = speed + turn;
			double rearLSpeed = speed + turn;
			double frontRSpeed = -speed + turn;
			double rearRSpeed = -speed + turn;
			double rsfl, rsrl;
			double rsfr, rsrr;
#ifndef PXA270
			printf(" %f %f\n", frontLSpeed, frontRSpeed);
#endif
			rsfl = rsrl = frontLSpeed * this->conf.rationSpeedToServo;
			rsfr = rsrr = frontRSpeed * this->conf.rationSpeedToServo;
#ifndef PXA270
			printf(" %f %f\n", rsfl, rsfr);
#endif
			if ((int) (frontLSpeed * this->conf.rationSpeedToServo)
					> this->conf.maxSpeed)
				rsfl = rsrl = this->conf.maxSpeed;
			if ((int) (frontLSpeed * this->conf.rationSpeedToServo)
					< -this->conf.maxSpeed)
				rsfl = rsrl = -this->conf.maxSpeed;
			if ((int) (frontRSpeed * this->conf.rationSpeedToServo)
					> this->conf.maxSpeed)
				rsfr = rsrr = this->conf.maxSpeed;
			if ((int) (frontRSpeed * this->conf.rationSpeedToServo)
					< -this->conf.maxSpeed)
				rsfr = rsrr = -this->conf.maxSpeed;

			for (int i = 0; i < 8; i++) {
				if (this->conf.frontL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.frontL, (int) rsfl);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}
#ifndef PXA270
			printf("%d\n", (int) rsfl);
#endif

			for (int i = 0; i < 8; i++) {
				if (this->conf.rearL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.rearL, (int) rsrl);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}
#ifndef PXA270
			printf("%d\n", (int) rsrl);
#endif

			for (int i = 0; i < 8; i++) {
				if (this->conf.frontR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.frontR, (int) rsrr);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}
#ifndef PXA270
			printf("%d\n", (int) rsfr);
#endif

			for (int i = 0; i < 8; i++) {
				if (this->conf.rearR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != (int) rsfl) {
						MFSetServoRotaSpd(this->conf.rearR, (int) rsrr);
						this->conf.speedAngle[i] = (int) rsfl;
					}
					break;
				}
			}

#ifndef PXA270
			printf("%d\n", (int) rsrr);
#endif
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
			for (int i = 0; i < 8; i++) {
				if (this->conf.servoFR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoFR, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}

			for (int i = 0; i < 8; i++) {
				if (this->conf.servoFL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoFL, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}

			for (int i = 0; i < 8; i++) {
				if (this->conf.servoRR == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoRR, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}

			for (int i = 0; i < 8; i++) {
				if (this->conf.servoRL == this->conf.id[i]) {
					if (this->conf.speedAngle[i] != pos) {
						MFSetServoPos(this->conf.servoRL, pos, 400);
						this->conf.speedAngle[i] = pos;
					}
					break;
				}
			}
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
		if (Message::MatchMessage(hdr, PLAYER_MSGTYPE_CMD,
				PLAYER_POSITION2D_CMD_VEL, this->Interface::addr)) {
			player_position2d_cmd_vel_t* pcmd =
					(player_position2d_cmd_vel_t*) data;
			double speed = sqrt(
					pcmd->vel.px * pcmd->vel.px + pcmd->vel.py * pcmd->vel.py);
			double turn = pcmd->vel.pa;
#ifndef PXA270
			printf("speed %f turn %f \n", speed, turn);
#endif
			double Left = speed + turn;
			double Right = turn - speed;
#ifndef PXA270
			printf("Left %f Right %f\n", Left, Right);
#endif
			double tl = Left * this->conf.rationSpeedToServo;
			double tr = Right * this->conf.rationSpeedToServo;
			if ((int) tl > this->conf.maxSpeed)
				tl = this->conf.maxSpeed;
			if ((int) tl < -this->conf.maxSpeed)
				tl = -this->conf.maxSpeed;
			if ((int) tr > this->conf.maxSpeed)
				tr = this->conf.maxSpeed;
			if ((int) tr < -this->conf.maxSpeed)
				tr = -this->conf.maxSpeed;

#ifndef PXA270
			printf("%f\n", tl);
#endif
#ifndef PXA270
			printf("%f\n", tr);
#endif
			MFSetServoRotaSpd(this->conf.servoFR, tr);
			MFSetServoRotaSpd(this->conf.servoRR, tr);
			MFSetServoRotaSpd(this->conf.servoFL, tl);
			MFSetServoRotaSpd(this->conf.servoRL, tl);

			return 0;
		}
		return -1;
	}
	return -1;
}

void InterfacePosition2d::Publish(void) {
	return;
}
