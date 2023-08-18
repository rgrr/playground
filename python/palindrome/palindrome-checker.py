n = 900
palindrome = []

while n < 1000:
    for k in range(900,1000):
        number = str(n * k)
        revnumber = number[::-1]
        if (number == revnumber):
            palindrome.append(number)
    n += 1

palindrome.sort()
print(f"The biggest palindrome is {palindrome[-1]}.")
