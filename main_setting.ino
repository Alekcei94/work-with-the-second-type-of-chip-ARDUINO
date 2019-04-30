/*
   0 - read temperature;
   1 - write coeff;
   2 - REZ commads;
   3 - write address;
   4 - check CRC and REZ
   5 - write EN2
   6 - command for temperature transfer;
   7 - write OTP;
*/

#include <OneWire.h>

OneWire dq(22);
OneWire dw(23);
OneWire de(24);
OneWire dr(25);
OneWire dt(26);
OneWire dy(27);
OneWire du(28);
OneWire di(29);
OneWire dp(30);
OneWire da(31);
OneWire dd(32);
OneWire df(33);
OneWire dg(34);
OneWire dh(35);
OneWire dj(36);
OneWire dk(37);
OneWire dl(38);
OneWire dz(39);
OneWire dx(40);
OneWire dc(41);
OneWire dv(42);
OneWire db(43);
OneWire dn(44);
OneWire dm(45);
OneWire dqq(46);
OneWire dqw(47);
OneWire dqe(48);
OneWire dqr(49);
OneWire dqt(50);
OneWire dqy(51);
OneWire dqu(52);
OneWire dqi(53);

byte bufData[9];
int binaryCode[16];
int collection[512];

void setup() {
  Serial.begin(9600);
}

void loop() {
  OneWire list[32] = {dq, dw, de, dr, dt, dy, du, di, dp, da, dd, df, dg, dh, dj, dk, dl, dz, dx, dc, dv, db, dn, dm, dqq, dqw, dqe, dqr, dqt, dqy, dqu, dqi};
  int commands = -1;
  commands = Serial.read();

  // Read temperature
  if (commands == '0') {
    //int int_number_pacet = form_interator_list();
    boolean flag_print_temperature = true;
    for (int int_number_pacet = 0; int_number_pacet < 32; int_number_pacet++) {
      read_data_from_port_arduino_and_tranform_KOD(list[int_number_pacet], flag_print_temperature);
    }

    //write K and B
  } else if (commands == '1') {
    int int_number_pacet = form_interator_list();
    sending_a_package_one_chip(list[int_number_pacet]);

    //set REZ
  } else if (commands == '2') {
    int int_number_pacet = form_interator_list();
    boolean flag_print_temperature = false;
    read_data_from_port_arduino_and_tranform_KOD(list[int_number_pacet], flag_print_temperature);
    write_config_REZ(list[int_number_pacet]);

    // set address
  } else if (commands == '3') {
    int int_number_pacet = form_interator_list();
    write_address(list[int_number_pacet]);

    //check ADDRESS
  } else if (commands == '4') {
    int int_number_pacet = form_interator_list();
    check_crc8_and_FAM(list[int_number_pacet]);

    // write enable2
  } else if (commands == '5') {
    int int_number_pacet = form_interator_list();
    write_en(list[int_number_pacet]);

    // transfer temperature
  } else if (commands == '6') {
    for (int i = 0; i < 32 ; i++) {
      give_me_temperature_in_chip(list[i]);
    }

    //write OTP;
  } else if (commands == '7') {
    int int_number_pacet = form_interator_list();
    write_OTP(list[int_number_pacet]);

  } else if (commands == '8') {

    int int_number_pacet = form_interator_list();
    check_real_coef(list[0]);
    //Serial.println(address_str);
  } else if (commands == '9') {
    byte bufData1[9];
    dq.reset();
    dq.write(0xe4);
    delay(1);
    dq.write(0x00);
    delay(1);
    dq.write(0xf8);
    //dq.write(0xc);
    delay(1);
    bufData1[1] = dq.read(); //low byte T
    bufData1[2] = dq.read(); //high byte T
    Serial.println(bufData1[1]);
    Serial.println(bufData1[2]);
  }
}

void test_OTP(OneWire ds) {
  byte bufData1[9];
  char list_coef[2];
  for (int i = 0; i < 2; i++) {
    delay(10);
    list_coef[i] = Serial.read();
  }
  dq.reset();
  dq.write(0xe4);
  delay(1);
  dq.write(list_coef[0]);
  delay(1);
  dq.write(list_coef[1]);
  //dq.write(0xc);
  delay(1);
  bufData1[1] = dq.read(); //low byte T
  bufData1[2] = dq.read(); //high byte T
  int T_kod = (bufData1[1] | ((bufData1[2]) << 8))*0.0625;
  Serial.println(bufData1[1], BIN);
  Serial.println(bufData1[2], BIN);
}

int form_interator_list() {
  byte number_pacet = 0;
  int int_number_pacet = -1;
  delay(10);
  number_pacet = Serial.read();
  delay(10);
  int_number_pacet = (int)number_pacet;
  return int_number_pacet;
}

void write_OTP(OneWire ds) {
  char list_coef[3];
  for (int i = 0; i < 3; i++) {
    delay(10);
    list_coef[i] = Serial.read();
  }
  ds.reset();
  ds.write(0xA8);
  delay(1);
  for (int i = 0; i < 3; i++) {
    ds.write(list_coef[i]);
    delay(1);
  }
  ds.write(0xff);
  /*dq.reset();
  dq.write(0xA8);
  dq.write(0x1);
  dq.write(0x1);
  dq.write(0x1);
  dq.write(0xff);*/
}

void print_one_chip_temperature(OneWire ds) {
  //read_data_from_port_arduino_and_tranform_KOD(ds);
}

void read_data_from_port_arduino_and_tranform_KOD(OneWire ds, boolean flag_print_temperature) {
  char notInvBinaryCodeChar[16];
  int variableList[16];

  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);

  bufData[1] = ds.read();
  bufData[2] = ds.read();
  int T_kod = 0;
  T_kod = (bufData[1] | ((bufData[2]) << 8));

  for (int i = 0; i < 16; i++) { //reset notInvBinaryCodeChar
    notInvBinaryCodeChar[i] = 0;
  }
  itoa(T_kod, notInvBinaryCodeChar, 2); //convert T_kod in char notInvBinaryCodeChar

  for (int i = 0; i < 16; i++) { //reset list
    binaryCode[i] = 0;
  }
  for (int i = 0; i < 16; i++) { //convert char in int (!-48!)
    binaryCode[i] = int(notInvBinaryCodeChar[i]) - 48;
  }
  for (int i = 0; i < 16; i++) { //reset variableList
    variableList[i] = 0;
  }
  int variable = 0;
  int variableOld = 15;
  for (int i = 15; i >= 0; i--) { //work (-48)
    if (binaryCode[i] < 0) {
      variableList[variable] = 0;
      variable = variable + 1;
    } else {
      variableList[variableOld] = binaryCode[i];
      variableOld = variableOld - 1;
    }
  }
  for (int i = 0; i < 16; i++) {
    binaryCode[i] = variableList[i];
  }
  for (int i = 5; i < 16; i++) { //invert
    if (binaryCode[i] == 0) {
      binaryCode[i] = 1;
    } else {
      binaryCode[i] = 0;
    }
  }
  if (flag_print_temperature) {
    for (int i = 0; i < 16; i++) {
      Serial.print(binaryCode[i]);
    }
    Serial.println();
  }
}

void write_config_REZ(OneWire ds) {
  for (int i = 4; i < 16; i++) {
    if (binaryCode[i] == 1) {
      ds.reset();
      ds.write(0x78);
      delay(1);
      //ds.write(0x7);
      //Serial.println(i);
      switch (28 - i) {
        case 23:
          ds.write(0x0);
          break;
        case 22:
          ds.write(0x1);
          break;
        case 21:
          ds.write(0x2);
          break;
        case 20:
          ds.write(0x3);
          break;
        case 19:
          ds.write(0x4);
          break;
        case 18:
          ds.write(0x5);
          break;
        case 17:
          ds.write(0x6);
          break;
        case 16:
          ds.write(0x7);
          break;
        case 15:
          ds.write(0x8);
          break;
        case 14:
          ds.write(0x9);
          break;
        case 13:
          ds.write(0x10);
          break;
        case 12:
          ds.write(0x11);
          break;
        case 11:
          ds.write(0x12);
          break;
      }
      break;
    }
  }
}

void write_address(OneWire ds) {
  char list_adres[8];
  for (int i = 0; i < 8; i++) {
    delay(10);
    list_adres[i] = Serial.read();
  }
  ds.reset();
  ds.write(0x38);
  for (int i = 0; i < 8; i++) {
    ds.write(list_adres[i]);
  }
}

void check_real_coef(OneWire ds) {
  give_me_temperature_in_chip(ds);
  delay(3000);
  int T_kod_check_coef;
  byte bufData_check_coef[9];  // буфер данных
  float temperature_check_coef;
  ds.reset();
  ds.write(0xCC); //skip rom
  ds.write(0xBE); //read_scratch

  bufData_check_coef[1] = ds.read(); //low byte T
  bufData_check_coef[2] = ds.read(); //high byte T

  temperature_check_coef = (bufData_check_coef[1] | ((bufData_check_coef[2]) << 8)) * 0.0625;
  Serial.print(temperature_check_coef);
}

void check_crc8_and_FAM(OneWire ds) {
  byte get_address_chip[8];

  ds.reset();
  ds.write(0x33); //skip rom

  get_address_chip[0] = ds.read();
  get_address_chip[1] = ds.read();
  get_address_chip[2] = ds.read();
  get_address_chip[3] = ds.read();
  get_address_chip[4] = ds.read();
  get_address_chip[5] = ds.read();
  get_address_chip[6] = ds.read();
  get_address_chip[7] = ds.read();
  if (get_address_chip[7] != 0) {
    if (OneWire::crc8(get_address_chip, 7) != get_address_chip[7]) {
      Serial.println('0');
    } else {
      Serial.println('1');
    }
    for (int i = 0; i < 8; i++) {
      Serial.println(get_address_chip[i]);
    }
  } else {
    Serial.println('0');
    for (int i = 0; i < 8; i++) {
      Serial.println('0');
    }
  }
}

void write_en(OneWire ds) {
  ds.reset();
  ds.write(0x68);
  ds.write(0xff);
}

void sending_a_package_one_chip(OneWire ds) {
  char list_coef[4];
  for (int i = 0; i < 4; i++) {
    delay(10);
    list_coef[i] = Serial.read();
  }
  ds.reset();
  ds.write(0x18);
  for (int i = 0; i < 4; i++) {
    ds.write(list_coef[i]);
  }
  ds.write(0x1);
}

void give_me_temperature_in_chip(OneWire ds) {
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);
}
