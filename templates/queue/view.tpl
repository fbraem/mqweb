<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
 <head>
	<title>MQWeb :: <?= mqweb.qmgr ?> ::  <?= queue.QName.value ?></title>
	<link	rel="shortcut	icon"	href="/static/css/images/favicon.ico"	/>
	<link	rel="stylesheet" href="/static/css/style.css"	type="text/css"	media="all"	/>
	<link	rel="stylesheet" href="/static/css/formalize.css"	type="text/css"	media="all"	/>
	<script src="/static/js/jquery-1.9.1.min.js" type="text/javascript"> </script>
	<script src="/static/js/jquery.form.js" type="text/javascript"> </script>
 </head>
 <body>
<body>
	<div id="ajaxOverlay"	style="display:none;">
	 <div	id="ajaxSpinner" style="background:url('/static/images/ajaxSpinner.gif') no-repeat center	center transparent;	width:100%;height:40px;">
	 </div>
	</div>
	<!-- Wrapper -->
	<div id="wrapper">
		<div class="shell">
			<div id="wrapper-top"></div>
			<!-- Wrapper Middle	-->
			<div id="wrapper-middle">
				<!-- Header	-->
				<div id="header">
					<!-- Logo	-->
					<div id="logo">
						<h1>MQWeb	-	<a href="/qmgr/view/<?=	mqweb.qmgr	?>"	title="home"><?= mqweb.qmgr	?></a></h1>
						<p class="slogan">Administrating Websphere MQ	with a browser</p>
					</div>
					<div class="socials">
					<a href="https://github.com/fbraem/mqweb"><img style="position:	absolute;	top: 0;	right: 0;	border:	0;"	src="https://s3.amazonaws.com/github/ribbons/forkme_right_red_aa0000.png"	alt="Fork	me on	GitHub"></a>
<!--
						<a title="Facebook"	class="facebook" href="#">facebook</a>
						<a title="Twitter" class="twitter" href="#">twitter</a>
						<a title="Digg"	class="digg" href="#">digg</a>
						<a title="Delicious" class="delicious" href="#">Delicious</a>
						<a title="Stumble	Upon"	class="su" href="#">su</a>
						<a title="RSS" class="rss" href="#">rss</a>	
-->
					</div>
					<div class="cl"></div>
				</div>
				<!-- END Header	-->
				<!-- Navigation	-->
				<div id="navigation">
					<ul>
						<li><a title="Home"	href="/qmgr/view/<?= mqweb.qmgr	?>">Home</a></li>
						<li><a title="Queues"	href="/queue/list/<?=	mqweb.qmgr	?>">Queues</a></li>
						<li><a title="Channels" href="/channel/index/<?=  mqweb.qmgr  ?>">Channels</a></li>
					</ul>
					<div class="cl"></div>
				</div>
				<!-- END Navigation	-->
				<!-- Main	-->
				<div id="main">
					<!-- Content -->
					<div id="content">
						<div class="post">
							<h3><strong><?= mqweb.qmgr	?></strong>	-	Queue - <?= queue.QName.value ?></h3>
							
							<?if queue.description ?>
								<em style="padding:5px;"><?= queue.QDesc.value ?></em>
							<?endif ?>
							<br />
							<div class="details">
								<?ifexist queue.DefinitionType ?>
									<h2>Type:</h2><div class="detail"><?= queue.QType.display ?> - <?= queue.DefinitionType.display ?> - <?= queue.Usage.display ?></div> <!-- detail -->
							 	<?else ?>
									<h2>Type:</h2><div class="detail"><?= queue.QType.display ?> <?= queue.Usage.display ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.CurrentQDepth ?>
									<h2>Depth:</h2><div class="detail"><?= queue.CurrentQDepth.value ?> / <?ifexist queue.MaxQDepth ?><?= queue.MaxQDepth.value ?><?else ?>-<?endif ?></div> <!-- detail -->
							 	<?else ?>
									<h2>Depth:</h2><div class="detail">- / <?ifexist queue.MaxQDepth ?><?= queue.MaxQDepth.value ?><?else ?>-<?endif ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.OpenInputCount ?>
									<h2>MQOPEN:</h2><div class="detail"><?= queue.OpenInputCount.value ?> for Input, <?= queue.OpenOutputCount.value ?> for Output</div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.InhibitGet ?>
									<h2>Operations:</h2><div class="detail">Get <?= queue.InhibitGet.display ?> - Put <?= queue.InhibitPut.display ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.CreationDate ?>
									<h2>Creation Date:</h2><div class="detail"><?= queue.CreationDate.value ?> <?= queue.CreationTime.value ?></div> <!-- detail -->
								<?endif ?>
								<h2>Alteration Date:</h2><div class="detail"><?= queue.AlterationDate.value ?> <?= queue.AlterationTime.value ?></div> <!-- detail -->
								<?if queue.BackoutRequeueName.value ?>
									<h2>Backout:</h2>
									<div class="detail">
										<a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.BackoutRequeueName.value ?>"><?= queue.BackoutRequeueName.value ?></a> - Threshold: <?= queue.BackoutThreshold.value?> - Harden: <?= queue.HardenGetBackout.display ?>
									</div> <!-- detail -->
								<?endif ?>
								<?if queue.BaseQName ?>
									<h2>Target Queue:</h2><div class="detail"><a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.BaseQName.value ?>"><?= queue.BaseQName.value ?></a></div> <!-- detail -->
								<?endif ?>
								<h2>Default Binding:</h2><div class="detail"><?= queue.DefBind.display ?></div> <!-- detail -->
								<?ifexist queue.DefInputOpenOption ?>
									<h2>Default Input Open:</h2><div class="detail"><?= queue.DefInputOpenOption.display ?></div> <!-- detail -->
								<? endif ?>
								<?ifexist queue.DistLists ?>
									<h2>Distribution List:</h2><div class="detail"><?= queue.DistLists.display ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.DefPersistence ?>
									<h2>Default Persistence:</h2><div class="detail"><?= queue.DefPersistence.display ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.DefPriority ?>
									<h2>Default Priority:</h2><div class="detail"><?= queue.DefPriority.value ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.DefReadAhead ?>
									<h2>Default Read Ahead:</h2><div class="detail"><?= queue.DefReadAhead.display ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.DefaultPutResponse ?>
									<h2>Default Put Response Type:</h2><div class="detail"><?= queue.DefaultPutResponse.display ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.QDepthHighEvent ?>
									<h2>Depth Hight Event:</h2><div class="detail"><?= queue.QDepthHighEvent.display ?> - Limit: <?= queue.QDepthHighLimit.value ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.QDepthLowEvent ?>
									<h2>Depth Low Event:</h2><div class="detail"><?= queue.QDepthLowEvent.display ?> - Limit: <?= queue.QDepthLowLimit.value ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.QDepthMaxEvent ?>
									<h2>Depth Max Event:</h2><div class="detail"><?= queue.QDepthMaxEvent.display ?></div> <!-- detail -->
								<?endif ?>
								<?if queue.InitiationQName.value ?>
									<h2>Initiation Queue:</h2><div class="detail"><a href="/queue/view/<?= mqweb.qmgr ?>/<?= queue.InitiationQName.value ?>"><?= queue.InitiationQName.value ?></a></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.MaxMsgLength ?>
									<h2>Maximum Message Length:</h2><div class="detail"><?= queue.MaxMsgLength.value ?></div> <!-- detail -->
								<?endif ?>
								<?ifexist queue.MsgDeliverySequence ?>
									<h2>Message Delivery:</h2><div class="detail"><?= queue.MsgDeliverySequence.display ?></div> <!-- detail -->
								<?endif ?>
								<?if queue.ProcessName.value ?>
									<h2>Process:</h2><div class="detail"><?= queue.ProcessName.value ?></div> <!-- detail -->
								<? endif ?>
								<?ifexist queue.TriggerControl ?>
									<h2>Trigger:</h2>
									<div class="detail">
										<?= queue.TriggerControl.display ?> - Type: <?= queue.TriggerType.display ?> - Depth: <?= queue.TriggerDepth.value ?> - Message Priority: <?= queue.TriggerMsgPriority.value ?> - Data: <?= Queue.TriggerData.value ?>
									</div> <!-- detail -->
								<? endif ?>
							</div> <!-- details -->
						</div>
					</div>
					<!-- END Content -->
					<!-- Sidebar -->
					<div id="sidebar">
						<div class="box">
							<h3>Our	Services</h3>
							<ul>
								<li><a title="List Queues	Containing Messages" href="#">List Non-empty Queues</a></li>
								<li><a title="Browse Messages" href="/message/index/<?= mqweb.qmgr	?>/<?= queue.QName.value ?>">Browse Messages</a></li>
							</ul>
						</div>
					</div>
					<!-- END Sidebar -->
					<div class="cl"></div>
					<div class="widgets">
						<div class="result" id="result">
							<div class="cl"></div>
						</div> <!-- result -->
						<div class="col	contacts"> </div> <!-- contacts -->
						<div class="cl"></div>
					</div> <! -- widgets -->
				</div> <!-- END Main	-->
			</div> <!-- END Wrapper Middle	-->
			<div id="wrapper-bottom"></div>
			<!-- Footer	-->
			<div id="footer">
				<p class="copy">Copyright	2012<span>|</span><a href="http://www.zumuta.be">www.zumuta.be</a>.	Design by	<a title="CSS	Free Templates with	jQuery Slider" href="http://css-free-templates.com/">CSS-Free-Templates.com</a></p>
			<!--	<p id="bottom-nav"><a	title="Home" href="#">Home</a><span>|</span><a title="About" href="#">About</a><span>|</span><a	title="Services" href="#">Services</a><span>|</span><a title="Projects"	href="#">Projects</a><span>|</span><a	title="Solutions"	href="#">Solutions</a><span>|</span><a title="Jobs"	href="#">Jobs</a><span>|</span><a	title="Blog" href="#"> Blog</a><span>|</span><a	title="Contacts" href="#">Contacts</a></p> -->
				<div class="cl"></div>
			</div>
			<!-- END Footer	-->
		</div>
	</div>
	<!-- END Wrapper -->
</body>
</html>