(*Require Import Coq.ZArith.Int.*)
(*
Inductive Pair : Set -> Set -> Set := 
 | p : forall (a:Set) (b:Set), a -> b-> Pair a b.
*)
Require Import Nat.
Definition walk (n:nat) (f:nat->nat) : nat:=
  let fix walk_loop (j:nat) (m:nat) : nat := match j with
    | 0 => m
    | S pr => walk_loop pr (f m)
   end in
     walk_loop n 0.
Function A (n : nat) (i : nat) (j: nat) : nat
  := match i with
     | 0 => 0
     | S pr => if (j =? walk n (fun k => A n pr k)) then S (A n pr j) else A n pr j
    end.
Compute (A 21 9 0, A 21 9 1,A 21 9 2).
Compute (A 21 6 3).