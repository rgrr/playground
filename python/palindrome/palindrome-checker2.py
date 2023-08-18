



def is_palindrome(n: int) -> bool:
    s = str(n)
    rev_s = s[::-1]
    return s == rev_s



n = 900
max_palindrome = 0

while n < 1000:
    for k in range(900,1000):
        if n * k > max_palindrome  and  is_palindrome(n * k):
            max_palindrome = n * k
    n += 1

print(f"The biggest palindrome is {max_palindrome}.")
