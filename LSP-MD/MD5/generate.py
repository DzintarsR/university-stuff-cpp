import sys
from random import randint

def create_values(size):
	created = 0
	while created <= size:
		value = randint(3, 500)
		created += value
		print value

create_values(int(sys.argv[1]))