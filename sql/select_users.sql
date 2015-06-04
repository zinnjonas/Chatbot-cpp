SELECT jabber.id, jabber, alias, first_name, last_name, rights, blocked 
FROM block 
    join jabber
       on block.id = jabber.id
    join right 
       on block.id = right.id
    join user
       on block.id = user.id
