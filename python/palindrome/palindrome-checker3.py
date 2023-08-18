import time


def is_palindrome(n: int) -> bool:
    s = str(n)
    rev_s = s[::-1]
    return s == rev_s



start_time = time.time()

max_n = 999999
max_palindrome = 0

for n in range(max_n, 0, -1):
    if n * max_n > max_palindrome:
        for k in range(max_n, n, -1):
            if n * k > max_palindrome  and  is_palindrome(n * k):
                print(f"{n} * {k} = {n *k}")
                max_palindrome = n * k
                break

elapsed_time = time.time() - start_time
print(f"The biggest palindrome with k*n and k,n <= {max_n} is {max_palindrome}.  Time: {elapsed_time:.3f}s")
