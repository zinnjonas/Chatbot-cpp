CREATE TABLE clock (
    id INTEGER NOT NULL,
    continuous INTEGER NOT NULL DEFAULT 0,
    timer INTEGER NOT NULL,
    intervall INTEGER NOT NULL,
    username TEXT NOT NULL,
    func TEXT NOT NULL,
    args TEXT NOT NULL DEFAULT "",
    reputation INTEGER NOT NULL,
    PRIMARY KEY(id)
);
