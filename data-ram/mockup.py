#!/usr/bin/env python
# -*- coding: utf-8 -*-


def calcola_codice(nome, cognome, giorno, mese, anno, sesso, comune, estero=False):

	print nome, cognome, giorno, mese, anno, sesso, comune, estero
	
	codice = []

	nome = nome.upper()
	cognome = cognome.upper()

        comune = comune.upper()
	
	# Cognome (tre): solo consonanti
	count = 0
	for cogn in cognome.split(" "):
		for lettera in cogn:
			if lettera not in ("A", "E", "I", "O", "U") and count < 3:
				codice.append(lettera)
				count += 1
	# Se non sono tre, si aggiungono le vocali
	for cogn in cognome.split(" "):
		for lettera in cogn:
			if lettera in ("A", "E", "I", "O", "U") and count < 3:
				codice.append(lettera)
				count += 1
	# Se non sono tre, si aggiunge la X
	while count < 3:
		codice.append("X")
		count += 1


	# Nome (tre): solo consonanti (prima, terza, quarta se >= 4; prime tre se < 4)
	lettere = []
	for nom in nome.split(" "):
		for lettera in nom:
			if lettera not in ("A", "E", "I", "O", "U"):
				lettere.append(lettera)

	if len(lettere) > 4 or len(lettere) == 4:
		# prima terza e quarta
		codice.append(lettere[0])
		codice.append(lettere[2])
		codice.append(lettere[3])
	elif len(lettere) == 3:
		# prima seconda e terza
		codice += lettere
	else:
		# meno di tre, si riparte e si aggiungono le vocali	
		for nom in nome.split(" "):
			for lettera in nom:
				if lettera in ("A", "E", "I", "O", "U") and len(lettere) < 3:
					lettere.append(lettera)
		
		if len(lettere) < 3:
			# ancora meno di 3, aggiungiamo una X
			while len(lettere) != 3:
				lettere.append("X")
		
		codice += lettere

	# Data di nascita
	anno = anno[2:] # ultimi 2
	mesi = {"01":"A", "02":"B", "03":"C", "04":"D", "05":"E", "06":"H", "07":"L", "08":"M", "09":"P", "10":"R", "11":"S", "12":"T"}
	mese = mesi[mese]

	giorno = giorno
	if sesso == "F": # se femmina, si somma 40
		giorno = str(int(giorno) + 40)

	codice += [anno, mese, giorno]

	# Comune/Stato
	if not estero:
                from comuni import Comuni
                if not comune in Comuni: return None
		codice.append(Comuni[comune])
	else:
                from esteri import Esteri
                if not comune in Esteri: return None
		codice.append(Esteri[comune])


	# codice controllo

	pari = []
	dispari = []
	count = 0

	for lettera in "".join(codice):
		count += 1
		if count%2 == 0:
			# pari
			pari.append(lettera)
		else:
			dispari.append(lettera)
		
	dispari_control = {"0":"1", "1":"0", "2":"5", "3":"7", "4":"9", "5":"13", "6":"15", "7":"17", "8":"19", "9":"21", "A":"1", "B":"0", "C":"5", "D":"7", "E":"9", "F":"13", "G":"15", "H":"17", "I":"19", "J":"21", "K":"2", "L":"4", "M":"18", "N":"20", "O":"11", "P":"3", "Q":"6", "R":"8", "S":"12", "T":"14", "U":"16", "V":"10", "W":"22", "X":"25", "Y":"24", "Z":"23"}
	pari_control    = {"0":"0", "1":"1", "2":"2", "3":"3", "4":"4", "5":"5", "6":"6", "7":"7", "8":"8", "9":"9", "A":"0", "B":"1", "C":"2", "D":"3", "E":"4", "F":"5", "G":"6", "H":"7", "I":"8", "J":"9", "K":"10", "L":"11", "M":"12", "N":"13", "O":"14", "P":"15", "Q":"16", "R":"17", "S":"18", "T":"19", "U":"20", "V":"21", "W":"22", "X":"23", "Y":"24", "Z":"25"}

	dispari_somma = 0
	pari_somma = 0
	tutto = 0
		
	# Facciamo la somma
	for lettera in dispari:
		dispari_somma += int(dispari_control[lettera])
	for lettera in pari:
		pari_somma += int(pari_control[lettera])
		
	tutto = pari_somma + dispari_somma
	tutto %= 26 # resto della divisione per 26

	tutto_control = {"0":"A", "1":"B", "2":"C", "3":"D", "4":"E", "5":"F", "6":"G", "7":"H", "8":"I", "9":"J", "10":"K", "11":"L", "12":"M", "13":"N", "14":"O", "15":"P", "16":"Q", "17":"R", "18":"S", "19":"T", "20":"U", "21":"V", "22":"W", "23":"X", "24":"Y", "25":"Z"}

	codice.append(tutto_control[str(tutto)])

	# omocodia not handled
	return "".join(codice)

