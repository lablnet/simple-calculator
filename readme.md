# Calculator like an interpreter

i = int(input("Enter first number"))
o = input("Enter the operation")
v = int(input("Enter second number"))

if o=="+":
  print("Your answer is:",i + v)
elif o=="-":
  print("Your answer is:",i - v) 
elif o=="*":
  print("Your answer is:",i * v)
elif o=="/":
  print("Your answer is:",i / v)
else:
  print("Invalid operation")
