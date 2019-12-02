let votes = {
  toby: [],
  scout: []
};

const showPets = function (req, res) {
  res.render('home', {
    votes: votes
  });
};


const petVote = function (req, res) {
  const thisUser = req.cookies["connect.sid"];
  
  if (votes.toby.includes(thisUser) || votes.scout.includes(thisUser))
  {
    req.session.flash = "Sorry, you have already voted!";
  }
  else
  {
    if (req.params.pet == "toby")
    {
      votes.toby.push(thisUser);
      req.session.flash = "Thank you for voting for Toby!";
    }
    else if (req.params.pet == "scout")
    {
      votes.scout.push(thisUser)
      req.session.flash = "Thank you for voting for Scout!";
    }
    else
    {
      req.session.flash = "Sorry, that pet is not on the ballot!";
    }
  }

  res.redirect("/");
};

module.exports = {
  showPets,
  petVote
};
