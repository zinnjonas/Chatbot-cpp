SELECT count(type) as 'exists'
FROM sqlite_master
WHERE type='table' and name='$1';