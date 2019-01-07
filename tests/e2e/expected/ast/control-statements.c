Block
  Declaration 1 x =
    Const 2.000000
Loop
  Init
    Declaration 0 i =
      Const 0.000000
  Condition
    Binary op lt
      Symbol 'i'
      Const 10.000000
  Body
    Block
      If
        Binary op gt
          Symbol 'x'
          Const 100.000000
      Then
        Block
          Assign x =
            Const 0.000000
          Continue
      Assign x =
        Binary op mul
          Symbol 'x'
          Symbol 'i'
  End
    Unary op incr
      Symbol 'i'
Loop
  Body
    Block
      Assign x =
        Binary op mul
          Symbol 'x'
          Const 2.000000
      If
        Binary op lt
          Symbol 'x'
          Const 10000.000000
      Then
        Block
          Assign x =
            Binary op sub
              Symbol 'x'
              Const 1.000000
      Else
        Block
          Return
            Symbol 'x'
