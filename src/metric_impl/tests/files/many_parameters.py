def __test_multiparameters__(a, b, d: int, c=5, e: int = 6, *args, **kwargs):
    assert a + b == c