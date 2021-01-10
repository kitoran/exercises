Require Import Coq.Init.Specif.
Require Import Coq.Logic.JMeq.
Require Import Classical.
(*Section SetTheory. (* Termless *)

End SetTheory.*)
Section SetTheory. (* Termful, background-equality *)
Axiom MSet : Type.
Axiom m : MSet -> MSet -> Prop. (*SProp????*)
Definition isEmpty x :=  forall (f:MSet), ~(m f x).
Axiom Empty : {x:MSet & forall (f:MSet), ~(m f x)}.
Axiom Pair : forall (a:MSet) (b:MSet), {c:MSet & forall t, m t c -> {t = a} + {t = b}}.
Axiom Extensionality : forall (a:MSet) (b:MSet), (forall c, m c a <-> m c b) -> a = b.
Definition rewr : forall a b, a = b -> (a) -> (b ). intros.
rewrite <- H. assumption. Defined.
Theorem het : forall a b (c:a) (d:b), JMeq c d -> {t:a = b & d = rewr a b t c}.
intros. destruct H. exact (existT (fun x => c = rewr a a x c) eq_refl eq_refl). Qed.

Axiom Uniquness : forall phi (eqq: forall a b, (phi a /\ phi b -> a = b)), 
                              (forall a (b:{x:Type & phi x}), a = b).
 match (eqq a b (conj pa pb)) with
                            | eq_refl =>
existT phi a = existT phi rewr a  b t
Axiom Uniquness : forall c phi (eqq: forall (a:c) (b:c), (phi a /\ phi b -> a = b)), 
                              (forall a (pa:phi a) (pb:phi a),
                                   pa = pb     ). (*proof irrelevance for defining/characteristic properties, like empty*)
Definition rewr : forall a b, a = b -> (a -> Type) -> (b -> Type).
auto. Defined.
Theorem eqsig : forall a b c d, {x:a & c x} = {y:b & d y} -> {t:a = b & d = rewr a b t c}.
Definition create_empty a := existT (isEmpty) a.
Definition empty_ergo_empty : forall a, isEmpty a -> {x:MSet & forall (f:MSet), ~(m f x)}.
intros.
Check H.
exact (existT isEmpty a H).
Defined.

Definition only_empty_another : forall a, isEmpty a -> a = (projT1 Empty).
intros.
apply Extensionality.
intros.
split; intros.
contradiction (H c H0).
exfalso.
contradiction ((projT2 Empty) c H0 ).
Defined.

Definition only_empty : forall a (n : forall f : MSet, ~ m f a) (ex:{x : MSet & forall f : MSet, ~ m f x}), isEmpty a.
intros.
unfold isEmpty.
intros.
exact (n f).
Defined.
Definition another_empty : forall x n,  (create_empty x n) = Empty.
intros.
unfold create_empty.
unfold isEmpty.

apply Extensionality.
(*Definition nonEmpty : forall a, ~(isEmpty a) -> {r:MSet & m r ( a)}.
intros.
unfold isEmpty in H.*)
Definition nonEmpty : forall a, ~(a = Empty) -> {r:MSet & m r (projT1 a)}.
intros.
destruct a.
simpl.
Check (create_empty x n).
Check (only_empty x n (create_empty x n)).


Definition intersectPred a b c := forall x, m x c <-> m x a /\ m x b.
Axiom Foundation : forall (a:MSet), {}.

End SetTheory.
Theorem Choice : forall a, (forall b:a, {c:b & True}) 
                     -> {r: (forall b:a, b) & True}.