removeEven :: [Int] -> [Int]
removeEven []             = []
removeEven [a]            = [a]
removeEven (a : _ : rest) = a : removeEven rest

removeOdd :: [Int] -> [Int]
removeOdd []             = []
removeOdd [_]            = []
removeOdd (_ : b : rest) = b : removeOdd rest

removeThird :: [Int] -> [Int]
removeThird (a : b : _ : rest) = a : b : removeThird rest
removeThird rest               = rest

step1 :: [Int] -> Int
step1 numbers = if length numbers == 1 then head numbers else step2 (removeEven numbers)

step2 :: [Int] -> Int
step2 numbers = if length numbers == 1 then head numbers else step3 (removeOdd numbers)

step3 :: [Int] -> Int
step3 numbers = if length numbers == 1 then head numbers else step1 (removeThird numbers)

main :: IO ()
main = do
  n <- readLn
  print (step1 [1 .. n])