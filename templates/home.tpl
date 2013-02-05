<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html lang="en-US" xmlns="http://www.w3.org/1999/xhtml" dir="ltr">
<head>
	<title>MQWeb - <?= qmgr.QMgrName.value ?></title>
	<meta http-equiv="Content-type" content="text/html; charset=utf-8" />
	<link rel="shortcut icon" href="/static/css/images/favicon.ico" />
	<link rel="stylesheet" href="/static/css/style.css" type="text/css" media="all" />
	<script src="/static/js/jquery-1.7.1.min.js" type="text/javascript" charset="utf-8"></script>
	<script type="text/javascript">
	  
	</script>
</head>
<body>
	<!-- Wrapper -->
	<div id="wrapper">
		<div class="shell">		
			<div id="wrapper-top"></div>
			<!-- Wrapper Middle -->
			<div id="wrapper-middle">
				<!-- Header -->
				<div id="header">
					<!-- Logo -->
					<div id="logo">
						<h1>MQWeb - <a href="/qmgr/view/<?= qmgr.QMgrName.value ?>" title="home"><?= qmgr.QMgrName.value ?></a></h1>
						<p class="slogan">Administrating Websphere MQ with a browser</p>
					</div>
					<div class="socials">
					<a href="https://github.com/fbraem/mqweb"><img style="position: absolute; top: 0; right: 0; border: 0;" src="https://s3.amazonaws.com/github/ribbons/forkme_right_red_aa0000.png" alt="Fork me on GitHub"></a>
<!--						<a title="Facebook" class="facebook" href="#">facebook</a>
						<a title="Twitter" class="twitter" href="#">twitter</a>
						<a title="Digg" class="digg" href="#">digg</a>
						<a title="Delicious" class="delicious" href="#">Delicious</a>
						<a title="Stumble Upon" class="su" href="#">su</a>
						<a title="RSS" class="rss" href="#">rss</a> -->						
					</div>
					<div class="cl"></div>								
				</div>
				<!-- END Header -->
				<!-- Navigation -->
				<div id="navigation">
					<ul>
						<li><a title="Home" href="/qmgr/view/<?= qmgr.QMgrName.value ?>">Home</a></li>
						<li><a title="Queues" href="/queue/list/<?= qmgr.QMgrName.value ?>">Queues</a></li>
					</ul>
					<div class="cl"></div>
				</div>
				<!-- END Navigation -->						
				<!-- Main -->
				<div id="main">
					<!-- Content -->
					<div id="content">
						<div class="post">
							<h3>QueueManager <strong><?= qmgr.QMgrName.value ?></strong></h3>
							<img src="/static/css/images/briefcase.jpg" alt="briefcase" />
							<p><strong><?= qmgr.QMgrDesc.value ?></strong></p>
							<div class="details" style="float:left;">
							  <h2>QueueManager ID</h2><div class="detail"><?= qmgr.QMgrIdentifier.value ?></div>
							  <? ifexist qmgr.CreationDate ?>
							    <h2>Creation Date</h2><div class="detail"><?= qmgr.CreationDate.value ?> <?= qmgr.CreationTime.value ?></div>
							  <? endif ?>
							  <h2>Alteration Date</h2><div class="detail"><?= qmgr.AlterationDate.value ?> <?= qmgr.AlterationTime.value ?></div>
							  <h2>Dead Letter Queue</h2><div class="detail"><a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a></div>
						  </div>
					  </div>
					</div>
					<!-- END Content -->
					<!-- Sidebar -->
					<div id="sidebar">
						<div class="box">
							<h3>Our Services</h3>
							<ul>
								<li><a title="List Queues Containing Messages" href="#">List Non-empty Queues</a></li>
								<li><a title="Ipsum is not simply" href="#">Ipsum is not simply</a></li>
								<li><a title="Dom text roots" href="#">Dom text roots</a></li>
								<li><a title="Popular belief lorem" href="#">Popular belief lorem</a></li>
								<li><a title="Ipsum is not simply" href="#">Ipsum is not simply</a></li>								
							</ul>
						</div>
					</div>
					<!-- END Sidebar -->
					<div class="cl"></div>
					<div class="widgets">
						<div class="col activities">
							<h3>Local Queues</h3>
							<p style="margin-bottom:5px">This is a list with local queues that contains at least one message.</p>
                <div>
                 <a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a> 2 <br />
                 <a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a> 3 <br />
                 <a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a> 15<br />
                 <a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a> 16<br />
                 <a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a> 201<br />
                 <a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a> 1<br />
                 <a href="/queue/view/<?=qmgr.QMgrName.value?>/<?= qmgr.DeadLetterQName.value ?>"><?= qmgr.DeadLetterQName.value ?></a> 12<br />
               </div>							
							
						</div>
						<div class="col about-us">
							<h3>Who We Are</h3>
							<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi sed est risus, in bibendum nibh. Suspendisse potenti. Sed eget enim id nisi pellentesque fringilla in at turpis amet</p>
							<a title="Read More" class="read-more" href="#">Read More</a>
						</div>
						<div class="col we-hire">
							<h3>We Hire</h3>
							<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi sed est risus, in bibendum nibh. Suspendisse potenti. Sed eget enim id nisi pellentesque fringilla in at turpis amet</p>
							<a title="Read More" class="read-more" href="#">Read More</a>
						</div>
						<div class="cl"></div>
					</div>
					<div class="widgets">
						<div class="projects">
							<h3>Latest Projects</h3>
							<div class="cl"></div>						
						</div>
						<div class="col contacts">
							<h3>Contact Us</h3>
							<p><strong>Website:</strong>&nbsp;<a href="http://www.zumuta.be">http://www.zumuta.be</a>
							<p><strong>Github:</strong>&nbsp;<a href="https://github.com/fbraem/mqweb/issues">https://github.com/fbraem/mqweb</a></p>
							<p><strong>Issues:</strong>&nbsp;<a href="https://github.com/fbraem/mqweb/issues">https://github.com/fbraem/mqweb/issues</a></p>
						</div>
						<div class="cl"></div>						
					</div>
				</div>
				<!-- END Main -->
			</div>
			<!-- END Wrapper Middle -->
			<div id="wrapper-bottom"></div>
			<!-- Footer -->
			<div id="footer">
				<p class="copy">Copyright 2012<span>|</span><a href="http://www.zumuta.be">www.zumuta.be</a>. Design by <a title="CSS Free Templates with jQuery Slider" href="http://css-free-templates.com/">CSS-Free-Templates.com</a></p>
			<!--	<p id="bottom-nav"><a title="Home" href="#">Home</a><span>|</span><a title="About" href="#">About</a><span>|</span><a title="Services" href="#">Services</a><span>|</span><a title="Projects" href="#">Projects</a><span>|</span><a title="Solutions" href="#">Solutions</a><span>|</span><a title="Jobs" href="#">Jobs</a><span>|</span><a title="Blog" href="#"> Blog</a><span>|</span><a title="Contacts" href="#">Contacts</a></p> -->
				<div class="cl"></div>
			</div>
			<!-- END Footer -->
		</div>
	</div>
	<!-- END Wrapper -->
</body>
</html>
