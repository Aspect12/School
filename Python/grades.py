
characterization = [0, 0, 0]
labels = ['fail', 'pass', 'excellent']
total, count = 0, 0

while True:
	try:
		grade = int(input("Enter a grade: "))
		count += 1

		if (grade < 60):
			characterization[0] += 1
		elif (grade < 90):
			characterization[1] += 1
		else:
			characterization[2] += 1

		total += grade

		if (count > 9):
			break
	except ValueError:
		print("Invalid input! The grade must be a number.")

print("\nThe average grade is:", int(total / count), "\n")

for i in range(len(characterization)):
	print(f"The percentage of students with the '{labels[i]}' grade is: {int((characterization[i] / count) * 100)}%")
