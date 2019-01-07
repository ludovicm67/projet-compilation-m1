Block
  Assign s1 =
    Unary op sqrt
      Const 9.000000
  Assign s2 =
    Unary op neg
      Symbol 's1'
  Assign s3 =
    Binary op add
      Const 4.000000
      Const 3.000000
  Assign resultat =
    Binary op mul
      Symbol 's2'
      Symbol 's3'
