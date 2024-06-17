
class Student:
	def __init__(self, st_id, name, surname, filename):
		self.st_id = st_id
		self.name = name
		self.surname = surname
		self.filename = filename

	def get_average(self):
		try:
			with open(self.filename, "r") as file:
				lines = file.readlines()
				grades = []

				for line in lines:
					grades.append(int(line))

				return sum(grades) / len(lines)
		except FileNotFoundError:
			print("File not found!")
		except ValueError:
			print("Invalid data in file!")
