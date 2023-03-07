void sendStringOverToCOM1()
{
	char toSend[] = "StringToSend";
	for (int i = 0; i < strlen(toSend); i++) {
		sendCharacter(toSend[i]);
	}
}

void sendCharacter(char c) {
	__outp(COM1_THR, c);
	while ((__inp(COM1_LSR) & LSR_THR_EMPTY) == 0); //Wait until the character is sent
}

void receiveStringOverCOM1() {
	char receivedString[] = "ReceivedString\\";
	for (int i = 0; i < strlen(receivedString); i++) {
		if (receivedString[i] != '\\') {
			receiveCharacter();

		}
	}
}

INT receiveCharacter() {
	while ((__inp(COM1_LSR) & LSR_DATA_READY) == 0);
	return __inp(COM1_RBR);
}